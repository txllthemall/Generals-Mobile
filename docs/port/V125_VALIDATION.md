# v125 device validation checklist

1. Launch base Zero Hour with the same game-data folder used for v123/v124 comparison.
2. Confirm startup reaches the main menu with no `Science ... not recognized` cascade and no `Technical Difficulties` dialog.
3. Confirm Setup -> Launch Game starts vanilla even after a prior mod launch.
4. Select a mod folder containing one loose override and verify the override is visible in game.
5. Select a mod folder containing a `.big` override and verify it wins over the base archive.
6. Clear the selected mod and confirm vanilla behavior returns without changing the base game folder.
