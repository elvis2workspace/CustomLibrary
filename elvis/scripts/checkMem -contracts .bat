set process=westone.md.enterprisecontracts

@adb shell dumpsys meminfo %process% | findstr "Pss"

pause