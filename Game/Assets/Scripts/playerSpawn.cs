using UnityEngine;
using System.Collections;

public class playerSpawn : MonoBehaviour {

	public Transform Player;

	// Use this for initialization
	void Awake () 
	{
		Object playah;

		playah = Instantiate(Player, new Vector3(-2.27F, -6.91F), new Quaternion(0, 0, 0, 0));
	}
	
	// Update is called once per frame
	void Update () 
	{
	
	}
}
