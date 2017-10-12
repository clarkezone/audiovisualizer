msbuild AudioVisualizer.sln /t:build /p:Configuration=Release;Platform=x86
msbuild AudioVisualizer.sln /t:build /p:Configuration=Release;Platform=x64
Tools\nuget.exe pack AudioAnalyzer.nuspec -o .\package -Version 0.0.5
