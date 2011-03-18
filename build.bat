msbuild /property:Configuration=Release /target:build
copy /y GwKeysGui\bin\Release\GwKeysGui.exe Release\GwKeysGui.exe
copy /y GwKeysGui\bin\Release\GwKeysGui.pdf Release\GwKeysGui.pdb