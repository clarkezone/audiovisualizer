@echo off

powershell -command "(New-Object System.Net.WebClient).DownloadFile('http://dist.nuget.org/win-x86-commandline/latest/nuget.exe', 'nuget.exe')" && (
	nuget.exe restore AudioVisualizer.sln && (
		msbuild AudioVisualizer.sln /t:build /p:Configuration=Release;Platform=x86 && (
			vstest.console /Platform:x86 /Framework:FrameworkUAP10 AudioVisualizer.test\bin\x86\Release\AudioVisualizer.test.build.appxrecipe && (
				msbuild AudioVisualizer.sln /t:build /p:Configuration=Release;Platform=x64 && (
					vstest.console /Platform:x86 /Framework:FrameworkUAP10 AudioVisualizer.test\bin\x86\Release\AudioVisualizer.test.build.appxrecipe && (
						Tools\nuget.exe pack AudioAnalyzer.nuspec -o .\package -Version %1 && ( echo "Package created" ) || (echo "Nuget failed")
					) || ( echo "x64 build failed" )
				) || ( echo "x64 tests failed")
			)  || (echo "x86 tests failed" )
		) || ( echo "x86 build failed" )
	) || ( echo "failed to restore packages" )
) || ( echo "failed to get nuget.exe")

