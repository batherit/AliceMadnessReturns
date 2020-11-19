// 명령어			옵션				원본이 있는 경로			복사본을 저장할 경로

xcopy			/y				.\Engine\Header\*.h			.\Reference\Header\
xcopy			/y				.\Engine\Header\*.inl		.\Reference\Header\


xcopy			/y				.\Engine\System\Code\*.h	.\Reference\Header\
xcopy			/y				.\Engine\Utility\Code\*.h	.\Reference\Header\

xcopy			/y				.\Engine\System\Bin\System.dll			.\Client\Bin\
xcopy			/y				.\Engine\Utility\Bin\Utility.dll		.\Client\Bin\
copy			/y				.\Engine\Resources\Bin\Resources.dll	.\Client\Bin\

xcopy			/y				.\Engine\System\Bin\System.dll			.\Tool_3D\Bin\
xcopy			/y				.\Engine\Utility\Bin\Utility.dll		.\Tool_3D\Bin\
copy			/y				.\Engine\Resources\Bin\Resources.dll	.\Tool_3D\Bin\

xcopy			/y				.\Engine\System\Bin\System.lib		.\Reference\Library\
xcopy			/y				.\Engine\Utility\Bin\Utility.lib	.\Reference\Library\