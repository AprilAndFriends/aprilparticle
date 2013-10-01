package com.example.aprilparticle.android.demoParticle;

public class DemoParticle extends net.sourceforge.april.android.Activity
{
	static
	{
		System.loadLibrary("demo_particle");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(net.sourceforge.april.android.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
