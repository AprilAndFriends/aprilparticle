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
		this.forceArchivePath(this.getPackageResourcePath()); // forces APK as archive file
		super.onCreate(savedInstanceState);
	}
	
}
