/*
** Command & Conquer Generals Zero Hour(tm)
** Copyright 2025 Electronic Arts Inc.
** GPL-3.0-or-later
*/

package com.generalsx.zerohour;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings;
import android.system.ErrnoException;
import android.system.Os;
import android.util.Log;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.File;

/** Pre-launch selector for an optional read-only secondary mod folder. */
public class LaunchActivity extends Activity {
    private static final String TAG = "GeneralsLaunch";
    private static final int REQUEST_MOD_FOLDER = 2101;
    private static final int REQUEST_STORAGE_PERMISSION = 2102;
    private static final String PREFS_NAME = "generalsx_mod_launcher";
    private static final String PREF_MOD_PATH = "mod_path";

    private boolean openPickerAfterPermission;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        showLaunchDialogOrSetup();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (openPickerAfterPermission && hasStorageAccess()) {
            openPickerAfterPermission = false;
            openModFolderPicker();
        }
    }

    private void showLaunchDialogOrSetup() {
        String gamePath = SetupActivity.getSavedGamePath(this);
        if (gamePath == null || !SetupActivity.isValidGameFolder(new File(gamePath))) {
            clearNativeModEnvironment();
            startActivity(new Intent(this, SetupActivity.class));
            finish();
            return;
        }

        String modPath = getSavedModPath();
        if (modPath != null && !new File(modPath).isDirectory()) {
            clearSavedModPath();
            modPath = null;
        }

        final String selectedMod = modPath;
        final String[] items = selectedMod != null
            ? new String[] {
                "Launch base game",
                "Launch selected mod\n" + selectedMod,
                "Select / change mod folder",
                "Clear selected mod",
                "Open Setup"
            }
            : new String[] {
                "Launch base game",
                "Select mod folder",
                "Open Setup"
            };

        new AlertDialog.Builder(this)
            .setTitle("Generals Mobile")
            .setMessage(selectedMod == null
                ? "Optional secondary mod folder: not selected"
                : "Secondary mod folder is enabled. Mod files override matching base-game files without modifying the base folder.")
            .setItems(items, (dialog, which) -> {
                if (selectedMod != null) {
                    switch (which) {
                        case 0: launchGame(null); break;
                        case 1: launchGame(selectedMod); break;
                        case 2: requestModFolderPicker(); break;
                        case 3:
                            clearSavedModPath();
                            clearNativeModEnvironment();
                            Toast.makeText(this, "Mod folder cleared", Toast.LENGTH_SHORT).show();
                            showLaunchDialogOrSetup();
                            break;
                        default:
                            clearNativeModEnvironment();
                            startActivity(new Intent(this, SetupActivity.class));
                            finish();
                            break;
                    }
                } else {
                    switch (which) {
                        case 0: launchGame(null); break;
                        case 1: requestModFolderPicker(); break;
                        default:
                            clearNativeModEnvironment();
                            startActivity(new Intent(this, SetupActivity.class));
                            finish();
                            break;
                    }
                }
            })
            .setOnCancelListener(dialog -> finish())
            .show();
    }

    private void launchGame(String modPath) {
        try {
            if (modPath != null && new File(modPath).isDirectory()) {
                Os.setenv("GENERALSX_MOD_DIR", new File(modPath).getAbsolutePath(), true);
                Log.i(TAG, "native mod overlay enabled: " + modPath);
            } else {
                Os.unsetenv("GENERALSX_MOD_DIR");
                Log.i(TAG, "native mod overlay disabled");
            }
        } catch (ErrnoException e) {
            Log.e(TAG, "failed to configure GENERALSX_MOD_DIR", e);
            Toast.makeText(this, "Could not configure mod overlay: " + e.getMessage(), Toast.LENGTH_LONG).show();
            return;
        }

        startActivity(new Intent(this, GeneralsZHActivity.class));
        finish();
    }

    private void clearNativeModEnvironment() {
        try {
            Os.unsetenv("GENERALSX_MOD_DIR");
        } catch (ErrnoException e) {
            Log.w(TAG, "failed to clear GENERALSX_MOD_DIR", e);
        }
    }

    private String getSavedModPath() {
        String value = getSharedPreferences(PREFS_NAME, MODE_PRIVATE).getString(PREF_MOD_PATH, null);
        if (value == null) return null;
        value = value.trim();
        return value.isEmpty() ? null : value;
    }

    private void saveModPath(String path) {
        getSharedPreferences(PREFS_NAME, MODE_PRIVATE).edit().putString(PREF_MOD_PATH, path).apply();
    }

    private void clearSavedModPath() {
        getSharedPreferences(PREFS_NAME, MODE_PRIVATE).edit().remove(PREF_MOD_PATH).apply();
    }

    private boolean hasStorageAccess() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            return Environment.isExternalStorageManager();
        }
        return ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
            == PackageManager.PERMISSION_GRANTED;
    }

    private void requestModFolderPicker() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            if (!Environment.isExternalStorageManager()) {
                openPickerAfterPermission = true;
                Toast.makeText(this, "Grant All files access, then return to select the mod folder", Toast.LENGTH_LONG).show();
                try {
                    Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                    intent.setData(Uri.parse("package:" + getPackageName()));
                    startActivity(intent);
                } catch (Exception e) {
                    startActivity(new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION));
                }
                return;
            }
        } else if (!hasStorageAccess()) {
            ActivityCompat.requestPermissions(this,
                new String[] { Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE },
                REQUEST_STORAGE_PERMISSION);
            return;
        }
        openModFolderPicker();
    }

    private void openModFolderPicker() {
        startActivityForResult(new Intent(this, FolderPickerActivity.class), REQUEST_MOD_FOLDER);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_STORAGE_PERMISSION) {
            boolean granted = grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED;
            if (granted) {
                openModFolderPicker();
            } else {
                Toast.makeText(this, "Storage permission is required to select a mod folder", Toast.LENGTH_LONG).show();
                showLaunchDialogOrSetup();
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_MOD_FOLDER) {
            if (resultCode == Activity.RESULT_OK && data != null) {
                String path = data.getStringExtra(FolderPickerActivity.EXTRA_SELECTED_PATH);
                if (path != null && new File(path).isDirectory()) {
                    saveModPath(path);
                    Toast.makeText(this, "Mod folder selected", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(this, "Selected mod folder is not accessible", Toast.LENGTH_LONG).show();
                }
            }
            showLaunchDialogOrSetup();
        }
    }
}
