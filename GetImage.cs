using UnityEngine;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Threading;
public class GetImage : MonoBehaviour {

	public GameObject Screen;
	string url = "192.168.123.102:8080/?action=snapshot";
	Texture2D img;


	// Use this for initialization
	void Start () {
		// StartCoroutine(StartImg());
	}
	
	// Update is called once per frame
	void Update () {
		StartCoroutine(StartImg());
	}

	
	IEnumerator StartImg()
	{
		// Start a download of the given URL
		WWW www = new WWW(url);
		
		// Wait for download to complete
		yield return www;
		
		
		// assign texture
		Renderer renderer = Screen.GetComponent<Renderer>();
		renderer.material.mainTexture = www.texture;
	}
}
