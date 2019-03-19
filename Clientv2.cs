using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Net.Sockets;
using System.Net;
using System.Text;

public class Clientv2 : MonoBehaviour {

	
	public GameObject view;
	private string coord;
	private float xR;
	private float yR;
	private float zR;

	private int front = 2048;
	private int offset1 = 180;
	private int offset2 = 360;

	private int xRo = 0;
	private int yRo = 0;
	private int zRo = 0;
	
	private string motor1 = "19";
	private int motorPos1 = 2048;
	private int motorPos2 = 2048;
	private string motor2 = "20";

	private string LeftRight = "";
	private string UpDown = "";
	//private string temp3 = "";
	static Socket sock;

	// Use this for initialization
	void Start () {
		sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 1520);
		try
		{
			sock.Connect(localEndPoint);
			
		}
		catch
		{
			Console.Write("UNable to connect");
		}
	}

	
	// Update is called once per frame
	void Update () {
		
		xR = view.transform.eulerAngles.x;
		xRo = (int)xR;
		yR = view.transform.eulerAngles.y;
		yRo = (int)yR;
		zR = view.transform.eulerAngles.z;

		///////////////////////////////
		xRo -= offset1;
		yRo -= offset1;

		if (xRo < 0) {
			xRo += offset2;
		}

		if (yRo < 0) {
			yRo += offset2;
		}

		motorPos1 = (((360 - yRo) * 2048) / 180);
		motorPos2 = (((360 - xRo) * 2048) / 180);
			
		/// //////////////////////////
		LeftRight = "" + motorPos1;
		UpDown = "" + motorPos2;
		
		coord = motor1 + "y" + motorPos1 + "y" + motor2 + "y" + motorPos2 + "y";
		Debug.Log (coord);
		byte[] data;
		data = Encoding.ASCII.GetBytes(coord);
		sock.Send(data);
		
		
		//For testing purposes
		//Console.Write("Dat Send!\r\n");
		//Console.Write("Press any key to continue...");
		//Console.Read();
		// sock.Close();
	}
}
