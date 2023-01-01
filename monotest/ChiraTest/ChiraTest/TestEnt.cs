using System;
using ChiraSharp;

namespace ChiraTest {
	public class TestEnt {
		string message;

		public TestEnt()
		{
			message = "Hello World!";
		}

		public void SpeakHello()
		{
			CE.Print (message);
		}
	}
}
