package com.example.aprilparticle.android.demoAprilUi;

public class DemoAprilUi extends com.googlecode.april.android.Activity
{
	static
	{
		System.loadLibrary("demo_aprilui");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.googlecode.april.android.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
