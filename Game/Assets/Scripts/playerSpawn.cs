using UnityEngine;
using System.Collections;

public class playerSpawn : MonoBehaviour {

	public Transform Player;

	// Use this for initialization
	void Awake () 
	{
		Object playah;

		playah = Instantiate(Player);
	}
	
	// Update is called once per frame
	void Update () 
	{
	
	}
}
