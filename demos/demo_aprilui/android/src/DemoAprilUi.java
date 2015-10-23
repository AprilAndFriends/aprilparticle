package com.aprilparticle.demoAprilUi;

public class DemoAprilUi extends com.april.Activity
{
	static
	{
		System.loadLibrary("demo_aprilui");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.april.NativeInterface.apkPath); // forces APK as archive file
	}
	
}
