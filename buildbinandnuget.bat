@echo off
msbuild AudioVisualizer.sln /t:build /p:Configuration=CIBUILD;Platform=x86 && (
	msbuild AudioVisualizer.sln /t:build /p:Configuration=CIBUILD;Platform=x64 && (
		Tools\nuget.exe pack AudioAnalyzer.nuspec -o .\package -Version %1 && (
			echo "Package created"
		) || ( 
			echo "x64 build failed"
		)
	)

) || (
	echo "x86 build failed"
)
