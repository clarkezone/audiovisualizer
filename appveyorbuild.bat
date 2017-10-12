echo %APPVEYOR_BUILD_VERSION%
call buildbinandnuget.bat %APPVEYOR_BUILD_VERSION%
appveyor PushArtifact .\package\UWPAudioVisualizer.%APPVEYOR_BUILD_VERSION%.nupkg