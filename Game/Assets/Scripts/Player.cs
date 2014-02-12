using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour 
{
		// Update is called once per frame
	void Update () 
	{

	}

  void FixedUpdate()
  {
    float h = Input.GetAxis ("Horizontal");

    rigidbody2D.AddForce (Vector2.right * h * 10);
  }
}
