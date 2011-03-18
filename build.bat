msbuild /property:Configuration=Release /target:build
copy /y NetGui\bin\Release\NetGui.exe Release\NetGui.exe
copy /y NetGui\bin\Release\NetGui.pdb Release\NetGui.pdb