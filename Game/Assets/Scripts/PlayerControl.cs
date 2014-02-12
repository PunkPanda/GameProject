using UnityEngine;
using System.Collections;

public class PlayerControl : MonoBehaviour 
{
  public float maxVelocity = 10f;
  public float maxSprint = 15f;
  public float sprintBoost = 1.5f;

		// Update is called once per frame
	void Update () 
	{

	}

  void FixedUpdate()
  {
    float h = Input.GetAxis ("Horizontal"); // Value of horizontal movement input
    float s = Input.GetAxis ("Sprint");     // Value of sprint key input

    if (s > 0) 
    {
      Debug.Log("Sprinting");

        // Move player (sprint)
      if (h * rigidbody2D.velocity.x < maxSprint)
      {
        rigidbody2D.velocity += Vector2.right * (h * sprintBoost);
      }

      if (Mathf.Abs (rigidbody2D.velocity.x) > maxSprint) 
      {
        rigidbody2D.velocity = new Vector2 (Mathf.Sign (rigidbody2D.velocity.x) * maxVelocity, rigidbody2D.velocity.y);
      }
    }
    else 
    {
      Debug.Log("Not sprinting");

        // Move player (no sprint)
      if (h * rigidbody2D.velocity.x < maxVelocity) 
      {
        rigidbody2D.velocity += Vector2.right * h;
      }

      if (Mathf.Abs (rigidbody2D.velocity.x) > maxVelocity) 
      {
        rigidbody2D.velocity = new Vector2 (Mathf.Sign (rigidbody2D.velocity.x) * maxVelocity, rigidbody2D.velocity.y);
      }
    }

    if (Mathf.Abs(h) < 1) 
    {
      rigidbody2D.velocity = new Vector2 (0, rigidbody2D.velocity.y);
    }
  }
}
