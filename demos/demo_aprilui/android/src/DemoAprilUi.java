package com.example.aprilparticle.android.demoAprilUi;

public class DemoAprilUi extends net.sourceforge.april.android.Activity
{
	static
	{
		System.loadLibrary("demo_aprilui");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(net.sourceforge.april.android.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
