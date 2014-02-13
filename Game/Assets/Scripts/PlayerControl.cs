using UnityEngine;
using System.Collections;

public class PlayerControl : MonoBehaviour 
{
  public float maxVelocity = 15f; // Max velocity when not sprinting
  public float maxSprint = 25f;   // Max velocity when sprinting
  public float sprintBoost = 2f;  // Boost multiplier sprinting gives
  public float jumpForce = 1f;    // Jump height/force
  public float airSlow = .75f;    // Horizontal velocity is multiplied by this when jumping
  
  private Transform groundCheck;   // Position to check if grounded

  public bool grounded = false;
  public bool jump = false;
  public bool sprint = false;
  public bool facingRight = false;

  void Awake()
  {
    // Setting up references.
    groundCheck = transform.Find("groundCheck");
  }

		// Update is called once per frame
  void Update () 
  {
    grounded = Physics2D.Linecast (transform.position, groundCheck.position, 1 << LayerMask.NameToLayer ("Ground"));
    
    if (Input.GetButtonDown("Jump") && grounded)
    {
      jump = true;
    }
    
    sprint = Input.GetButton ("Sprint");     // Value of sprint key input
  }

  void FixedUpdate()
  {
    float h = Input.GetAxis ("Horizontal"); // Value of horizontal movement input

    if (sprint && grounded) 
    {
        // Move player (sprint)
      if (h * rigidbody2D.velocity.x < maxSprint)
      {
        Debug.Log("Sprinting");
        rigidbody2D.velocity += Vector2.right * (h * sprintBoost);
      }

      if (Mathf.Abs (rigidbody2D.velocity.x) > maxSprint) 
      {
        Debug.Log("Max sprint");
        rigidbody2D.velocity = new Vector2 (Mathf.Sign (rigidbody2D.velocity.x) * maxVelocity, rigidbody2D.velocity.y);
      }
    }
    else if (grounded)
    {
      // Move player (no sprint)
      if (h * rigidbody2D.velocity.x < maxVelocity) 
      {
        Debug.Log("Not sprinting");
        rigidbody2D.velocity += Vector2.right * h;
      }

      if (Mathf.Abs (rigidbody2D.velocity.x) > maxVelocity) 
      {
        Debug.Log("Max velocity");
        rigidbody2D.velocity = new Vector2 (Mathf.Sign (rigidbody2D.velocity.x) * maxVelocity, rigidbody2D.velocity.y);
      }
    }

    if (Mathf.Abs(h) < 1 && grounded) 
    {
      rigidbody2D.velocity = new Vector2 (0, rigidbody2D.velocity.y);
    }
    
    if (jump)
    {
      rigidbody2D.AddForce(new Vector2(0f, jumpForce));
      
      rigidbody2D.velocity = new Vector2 (rigidbody2D.velocity.x * airSlow, rigidbody2D.velocity.y);
      
      jump = false;
    }
        
    if(h < 0 && !facingRight && grounded)
      Flip();
    else if(h > 0 && facingRight && grounded)
      Flip();
  }
    
  void Flip ()
  {
    // Switch the way the player is labelled as facing.
    facingRight = !facingRight;
      
    // Multiply the player's x local scale by -1.
    Vector3 theScale = transform.localScale;
    theScale.x *= -1;
    transform.localScale = theScale;
  }
}
