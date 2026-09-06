/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//  (c) 2001-2003 Electronic Arts Inc.
////////////////////////////////////////////////////////////////////////////////

///////// StdLocalFileSystem.cpp /////////////////////////
// Stephan Vedder, April 2025
////////////////////////////////////////////////////////////

#include "Common/AsciiString.h"
#include "Common/GameMemory.h"
#include "Common/PerfTimer.h"
#include "StdDevice/Common/StdLocalFileSystem.h"
#include "StdDevice/Common/StdLocalFile.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>

#ifndef _WIN32
#include <strings.h>

// GeneralsX @bugfix felipebraz 23/03/2026 Asset root fallback path for loose file lookups.
static std::filesystem::path s_assetFallbackPath;

// Generals Mobile @feature mod-overlay 06/09/2026
// Resolve a relative Windows-style game path below an explicit root. Android
// storage is case-sensitive even though Generals data was authored for
// Windows, so Linux/Android gets the same component-by-component
// case-insensitive fallback used for the normal asset root.
static std::filesystem::path resolveExistingPathFromRoot(
	const std::filesystem::path& root,
	const std::filesystem::path& relativePath)
{
	if (root.empty() || relativePath.empty() || relativePath.is_absolute()) {
		return std::filesystem::path();
	}

	std::filesystem::path normalized = relativePath.lexically_normal();
	if (!normalized.empty() && *normalized.begin() == "..") {
		return std::filesystem::path();
	}

	std::error_code ec;
	std::filesystem::path direct = root / normalized;
	if (std::filesystem::exists(direct, ec) && !ec) {
		return direct;
	}

#if defined(__linux__)
	std::filesystem::path current = root;
	for (const auto& part : normalized) {
		if (part == ".") {
			continue;
		}

		std::filesystem::path directPart = current / part;
		ec.clear();
		if (std::filesystem::exists(directPart, ec) && !ec) {
			current = directPart;
			continue;
		}

		std::filesystem::path matched;
		ec.clear();
		for (const auto& entry : std::filesystem::directory_iterator(current, ec)) {
			if (ec) {
				break;
			}
			if (strcasecmp(entry.path().filename().string().c_str(), part.string().c_str()) == 0) {
				matched = entry.path();
				break;
			}
		}
		if (matched.empty()) {
			return std::filesystem::path();
		}
		current = matched;
	}

	ec.clear();
	if (std::filesystem::exists(current, ec) && !ec) {
		return current;
	}
#endif

	return std::filesystem::path();
}

static std::filesystem::path getModOverlayPath(const std::filesystem::path& relativePath, Int access)
{
#if defined(__ANDROID__)
	if ((access & File::WRITE) || relativePath.is_absolute()) {
		return std::filesystem::path();
	}

	const char *modRootValue = getenv("GENERALSX_MOD_DIR");
	if (modRootValue == nullptr || modRootValue[0] == '\0') {
		return std::filesystem::path();
	}

	std::string rootString(modRootValue);
	std::replace(rootString.begin(), rootString.end(), '\\', '/');
	return resolveExistingPathFromRoot(std::filesystem::path(std::move(rootString)), relativePath);
#else
	(void)relativePath;
	(void)access;
	return std::filesystem::path();
#endif
}

static void collectFileList(
	const std::filesystem::path& actualDirectory,
	const std::filesystem::path& logicalDirectory,
	const std::filesystem::path& searchExt,
	FilenameList& filenameList,
	Bool searchSubdirectories,
	Bool logErrors)
{
	std::error_code ec;
	auto iter = std::filesystem::directory_iterator(actualDirectory, ec);
	if (ec) {
		if (logErrors) {
			DEBUG_LOG(("StdLocalFileSystem::getFileListInDirectory - Error opening directory %s", actualDirectory.string().c_str()));
		}
		return;
	}

	for (; iter != std::filesystem::directory_iterator(); ++iter) {
		const std::string filenameStr = iter->path().filename().string();
		if (filenameStr == "." || filenameStr == "..") {
			continue;
		}

		std::error_code entryEc;
		if (iter->is_directory(entryEc)) {
			if (searchSubdirectories && !entryEc) {
				collectFileList(iter->path(), logicalDirectory / iter->path().filename(), searchExt,
					filenameList, searchSubdirectories, logErrors);
			}
			continue;
		}

		if (!entryEc && iter->path().extension() == searchExt) {
			std::filesystem::path logicalFile = logicalDirectory / iter->path().filename();
			AsciiString newFilename(logicalFile.string().c_str());
			filenameList.insert(newFilename);
		}
	}
}
#endif

StdLocalFileSystem::StdLocalFileSystem() : LocalFileSystem()
{
}

StdLocalFileSystem::~StdLocalFileSystem() {
}

static std::filesystem::path fixFilenameFromWindowsPath(const Char *filename, Int access)
{
	std::string fixedFilename(filename);

#ifndef _WIN32
	std::replace(fixedFilename.begin(), fixedFilename.end(), '\\', '/');
#endif

	std::filesystem::path path(std::move(fixedFilename));

#ifndef _WIN32
	// Mod loose files are a read-only overlay. Never redirect writes and
	// never prefix absolute paths (BIG archive loading passes absolute paths).
	std::filesystem::path modPath = getModOverlayPath(path, access);
	if (!modPath.empty()) {
		return modPath;
	}

	std::error_code ec;
	if (!std::filesystem::exists(path, ec) &&
		((!(access & File::WRITE)) || ((access & File::WRITE) && !std::filesystem::exists(path.parent_path(), ec))))
	{
		if (!s_assetFallbackPath.empty() && path.is_relative()) {
			std::filesystem::path assetRootPath = resolveExistingPathFromRoot(s_assetFallbackPath, path);
			if (!assetRootPath.empty()) {
				return assetRootPath;
			}

			if (access & File::WRITE) {
				std::filesystem::path writePath = s_assetFallbackPath / path;
				std::error_code ecAsset;
				if (std::filesystem::exists(writePath.parent_path(), ecAsset) && !ecAsset) {
					return writePath;
				}
			}
		}

		if (!(access & File::WRITE) && path.is_relative()) {
			std::filesystem::path cwdFixed = resolveExistingPathFromRoot(std::filesystem::path("."), path);
			if (!cwdFixed.empty()) {
				return cwdFixed;
			}
			DEBUG_LOG(("StdLocalFileSystem::fixFilenameFromWindowsPath - Error finding file %s", filename));
			return std::filesystem::path();
		}
	}
#endif

	return path;
}

File * StdLocalFileSystem::openFile(const Char *filename, Int access, size_t bufferSize)
{
	if (strlen(filename) <= 0) {
		return nullptr;
	}

	std::filesystem::path path = fixFilenameFromWindowsPath(filename, access);
	if (path.empty()) {
		return nullptr;
	}

	if (access & File::WRITE) {
		std::filesystem::path dir = path.parent_path();
		std::error_code ec;
		if (!std::filesystem::exists(dir, ec) || ec) {
			if(!std::filesystem::create_directories(dir, ec) || ec) {
				DEBUG_LOG(("StdLocalFileSystem::openFile - Error creating directory %s", dir.string().c_str()));
				return nullptr;
			}
		}
	}

	StdLocalFile *file = newInstance( StdLocalFile );
	if (file->open(path.string().c_str(), access, bufferSize) == FALSE) {
		deleteInstance(file);
		file = nullptr;
	} else {
		file->deleteOnClose();
	}

	return file;
}

void StdLocalFileSystem::update()
{
}

void StdLocalFileSystem::init()
{
}

void StdLocalFileSystem::reset()
{
}

Bool StdLocalFileSystem::doesFileExist(const Char *filename) const
{
	std::filesystem::path path = fixFilenameFromWindowsPath(filename, 0);
	if(path.empty()) {
		return FALSE;
	}

	std::error_code ec;
	return std::filesystem::exists(path, ec);
}

void StdLocalFileSystem::getFileListInDirectory(const AsciiString& currentDirectory, const AsciiString& originalDirectory, const AsciiString& searchName, FilenameList & filenameList, Bool searchSubdirectories) const
{
	// v125 hotfix: keep the vanilla/base directory walk byte-for-byte equivalent
	// to v123. v124 replaced it with a recursive collector, which made an
	// absolute game-data root discover nested BIG archives that v123 never
	// treated as part of the primary ZH archive set. That changed INI precedence
	// and mixed base Generals definitions into Zero Hour. Mod-only discovery is
	// kept separate and is only added for relative loose-file directory scans.
	AsciiString asciisearch;
	asciisearch = originalDirectory;
	asciisearch.concat(currentDirectory);
	auto searchExt = std::filesystem::path(searchName.str()).extension();
	if (asciisearch.isEmpty()) {
		asciisearch = ".";
	}

	std::string fixedDirectory(asciisearch.str());

#ifndef _WIN32
	std::replace(fixedDirectory.begin(), fixedDirectory.end(), '\\', '/');

#if defined(__ANDROID__)
	std::filesystem::path logicalDirectory(fixedDirectory);
	const char *modRootValue = getenv("GENERALSX_MOD_DIR");
	if (modRootValue != nullptr && modRootValue[0] != '\0' && logicalDirectory.is_relative()) {
		std::string rootString(modRootValue);
		std::replace(rootString.begin(), rootString.end(), '\\', '/');
		std::filesystem::path modDirectory = std::filesystem::path(std::move(rootString)) / logicalDirectory;
		collectFileList(modDirectory, logicalDirectory, searchExt, filenameList, searchSubdirectories, FALSE);
	}
#endif
#endif

	Bool done = FALSE;
	std::error_code ec;

	auto iter = std::filesystem::directory_iterator(fixedDirectory.c_str(), ec);
	done = iter == std::filesystem::directory_iterator();

	if (ec) {
		DEBUG_LOG(("StdLocalFileSystem::getFileListInDirectory - Error opening directory %s", fixedDirectory.c_str()));
		return;
	}

	while (!done) {
		std::string filenameStr = iter->path().filename().string();
		if (!iter->is_directory() && iter->path().extension() == searchExt &&
			(strcmp(filenameStr.c_str(), ".") != 0 && strcmp(filenameStr.c_str(), "..") != 0)) {
			AsciiString newFilename = iter->path().string().c_str();
			if (filenameList.find(newFilename) == filenameList.end()) {
				filenameList.insert(newFilename);
			}
		}

		iter++;
		done = iter == std::filesystem::directory_iterator();
	}

	if (searchSubdirectories) {
		auto subIter = std::filesystem::directory_iterator(fixedDirectory, ec);

		if (ec) {
			DEBUG_LOG(("StdLocalFileSystem::getFileListInDirectory - Error opening subdirectory %s", fixedDirectory.c_str()));
			return;
		}

		done = subIter == std::filesystem::directory_iterator();

		while (!done) {
			std::string filenameStr = subIter->path().filename().string();
			if(subIter->is_directory() &&
				(strcmp(filenameStr.c_str(), ".") != 0 && strcmp(filenameStr.c_str(), "..") != 0)) {
				AsciiString tempsearchstr(filenameStr.c_str());
				getFileListInDirectory(tempsearchstr, originalDirectory, searchName, filenameList, searchSubdirectories);
			}

			subIter++;
			done = subIter == std::filesystem::directory_iterator();
		}
	}
}

Bool StdLocalFileSystem::getFileInfo(const AsciiString& filename, FileInfo *fileInfo) const
{
	std::filesystem::path path = fixFilenameFromWindowsPath(filename.str(), 0);
	if(path.empty()) {
		return FALSE;
	}

	std::error_code ec;
	auto file_size = std::filesystem::file_size(path, ec);
	if (ec) {
		return FALSE;
	}

	auto write_time = std::filesystem::last_write_time(path, ec);
	if (ec) {
		return FALSE;
	}

	auto time = write_time.time_since_epoch().count();
	fileInfo->timestampHigh = time >> 32;
	fileInfo->timestampLow = time & UINT32_MAX;
	fileInfo->sizeHigh = file_size >> 32;
	fileInfo->sizeLow = file_size & UINT32_MAX;
	return TRUE;
}

Bool StdLocalFileSystem::createDirectory(AsciiString directory)
{
	bool result = FALSE;
	std::string fixedDirectory(directory.str());
#ifndef _WIN32
	std::replace(fixedDirectory.begin(), fixedDirectory.end(), '\\', '/');
#endif
	if ((!fixedDirectory.empty()) && (fixedDirectory.length() < _MAX_DIR)) {
		std::filesystem::path path(std::move(fixedDirectory));
		std::error_code ec;
		result = std::filesystem::create_directory(path, ec);
		if (ec) {
			result = FALSE;
		}
	}
	return result;
}

AsciiString StdLocalFileSystem::normalizePath(const AsciiString& filePath) const
{
	std::string nonNormalized(filePath.str());
#ifndef _WIN32
	std::replace(nonNormalized.begin(), nonNormalized.end(), '\\', '/');
#endif
	std::filesystem::path pathNonNormalized(nonNormalized);
	return AsciiString(pathNonNormalized.lexically_normal().string().c_str());
}

#ifndef _WIN32
void StdLocalFileSystem::setAssetRootPath(const AsciiString& path)
{
	std::string p(path.str());
	std::replace(p.begin(), p.end(), '\\', '/');
	s_assetFallbackPath = std::filesystem::path(std::move(p));
	DEBUG_LOG(("StdLocalFileSystem::setAssetRootPath - asset fallback path set to '%s'", s_assetFallbackPath.string().c_str()));
}
#endif
