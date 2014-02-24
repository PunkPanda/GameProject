using UnityEngine;
using System.Collections;

public class Door : MonoBehaviour
{
	public string nextLocation = "0";

	bool e = false;

	void OnCollisionStay2D(Collision2D col)
	{
		if (col.gameObject.tag == "Player")
		{
			if (Input.GetButtonDown("Use"))
			{
				Application.LoadLevel(nextLocation);
			}
		}
	}

	void Update()
	{
		bool e = Input.GetButtonDown("Use");
	}
}