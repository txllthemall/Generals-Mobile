# v125 hotfix: restore Zero Hour BIG/INI precedence

v124 changed `StdLocalFileSystem::getFileListInDirectory()` from the v123 directory walk to a recursive collector used by the new mod overlay. On Android, an absolute primary game-data root could therefore discover nested `.big` archives that v123 did not include in the same primary archive set. That changed archive precedence and could mix base Generals INI definitions into Zero Hour.

Observed on-device symptoms included truncated/older definitions from files such as `Science.ini`, followed by `CommandSet.ini` references to command buttons/sciences that had not been registered and the generic `Technical Difficulties` dialog.

The v125 fix restores the v123 base directory walk for normal game-data enumeration. Mod-only recursive discovery remains separate and is only added to relative loose-file directory scans, so selecting no mod cannot change vanilla BIG ordering.

Validation target:

- Same game-data folder that launches on v123 must launch on v125 without `Science ... not recognized` cascades.
- Vanilla launch must not set or consume the mod overlay.
- Selected loose mod files must still override matching base files.
- Selected mod BIG archives must still load after base archives with overwrite priority.
