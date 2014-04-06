package com.example.aprilparticle.android.demoParticle;

public class DemoParticle extends com.googlecode.april.android.Activity
{
	static
	{
		System.loadLibrary("demo_particle");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.googlecode.april.android.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
