using System;
using ChiraSharp;

namespace ChiraTest {
	public class TestEnt {
		readonly string message;

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
