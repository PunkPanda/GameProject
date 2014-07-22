using UnityEngine;
using System.Collections;

public class PlayerMovement : MonoBehaviour 
{
		// movement config
	public float gravity = -15f;
	public float walkSpeed = 8f;
	public float sprintSpeed = 120f;
	public float crouchSpeed = 5f;
	public float groundDamping = 20f;
	public float inAirDamping = 5f;
	public float jumpHeight = 3f;
	public float horizontalAirVel = 0f;

	[HideInInspector]
	private float normalizedHorizontalSpeed = 0;
    
	private CharacterController2D _controller;
	private RaycastHit2D _lastControllerColliderHit;
	private Vector3 _velocity;

	private BoxCollider2D body;

	private Animator anim;

	private float squatTimer = 0;
	//private GameObject interact = null;

	enum PlayerState
	{
		STAND,
		CROUCH,
		CRAWL
	};
    
	PlayerState State;
    
		// bools
	bool jump = false;
	bool sprint = false;
    
	void Awake()
	{
			// Get the component from the object, contains functions and data used in movement
		_controller = GetComponent<CharacterController2D>();
		_controller.onControllerCollidedEvent += onControllerCollider;

			// Crouch junk
		body = gameObject.GetComponent<BoxCollider2D>();
		State = PlayerState.STAND;

			// listen to some events for illustration purposes
		_controller.onControllerCollidedEvent += onControllerCollider;
		/*_controller.onTriggerEnterEvent += onTriggerEnterEvent;
		_controller.onTriggerExitEvent += onTriggerExitEvent;*/

		anim = GetComponent<Animator>();
	}
    
    #region Event Listeners

	void onControllerCollider( RaycastHit2D hit )
	{
			// bail out on plain old ground hits
		if( hit.normal.y == 1f )
			return;
        
		// logs any collider hits if uncommented
		//Debug.Log( "flags: " + _controller.collisionState + ", hit.normal: " + hit.normal );
	}

	/*void onTriggerEnterEvent( Collider2D col )
	{
		interact = col.gameObject;
	}

	void onTriggerExitEvent (Collider2D col)
	{
		interact = null;
	}*/

	#endregion

	void Update()
	{
		/*if (State == PlayerState.STAND)
			Debug.Log("Stand");
		else if (State == PlayerState.CROUCH)
			Debug.Log("Crouch");
		else if (State == PlayerState.CRAWL)
			Debug.Log("Crawl");*/

			// Input
		if (Input.GetButtonDown("Jump") && _controller.isGrounded)
		{
			jump = true;
			anim.SetBool("Jump", true);
			anim.SetBool("Crouch", false);
		}

		sprint = Input.GetButton("Sprint");
 
		float h = Input.GetAxis("Horizontal");
		bool c = Input.GetButtonDown("CrouchDown");
		bool s = Input.GetButtonDown("StandUp");
		bool e = Input.GetButtonDown("Use");

		/*if (e)
		{
			if (interact.tag == "Door")
			{
				Application.LoadLevel(interact.guiText.text);
			}
		}*/

			// Create local velocity to be used in all further calculations
		_velocity = _controller.velocity;

			// Ground movement
		if (_controller.isGrounded)
		{
			_velocity.y = 0; // On the ground, no need for y
            
			if (c)
			{
				if (State != PlayerState.CRAWL)
				{
					anim.SetBool("Crouch", true);
					++State;
              
					body.size = body.size - new Vector2(0, 0.5f);
                    body.center = new Vector2(body.center.x, body.center.y - 0.25f);
                    //_controller.transform.position = new Vector3(_controller.transform.position.x, _controller.transform.position.y - 0.5f);
                    
                    _controller.totalHorizontalRays -= 2;
					//body.transform.position = new Vector3(body.transform.position.x, body.transform.position.y - 1); 
					//_controller.boxCollider.collider2D.transform.localScale = _controller.boxCollider.collider2D.transform.localScale - new Vector3(0, 0.5f, 0);
				}
			}
			else if (s || jump)
			{
				if (State != PlayerState.STAND)
				{
					anim.SetBool("Crouch", false);
					--State;
             
					body.size = body.size + new Vector2(0, 0.5f);
                    body.center = new Vector2(body.center.x, body.center.y + 0.25f);
                    //_controller.transform.position = new Vector3(_controller.transform.position.x, _controller.transform.position.y + 0.5f);
                    
                    _controller.totalHorizontalRays += 2;
					//_controller.boxCollider.collider2D.transform.localScale = _controller.boxCollider.collider2D.transform.localScale + new Vector3(0, 0.5f, 0);
				}
			}
			if (h > 0) // Right
			{
				normalizedHorizontalSpeed = 1;

					// Flip
				if (transform.localScale.x < 0f)
					transform.localScale = new Vector3(-transform.localScale.x, transform.localScale.y, transform.localScale.z);
			} 
			else if (h < 0) // Left
			{
				normalizedHorizontalSpeed = -1;

					// Flip
				if (transform.localScale.x > 0f)
					transform.localScale = new Vector3(-transform.localScale.x, transform.localScale.y, transform.localScale.z);
			} 
			else
			{
				normalizedHorizontalSpeed = 0;
			}
		} 
		else    // Aerial movement
		{
			if (_controller.collisionState.wasGroundedLastFrame)
			{
				if (sprint)
					horizontalAirVel = 0.5f * normalizedHorizontalSpeed*2;
				else
					horizontalAirVel = 0.5f * normalizedHorizontalSpeed;
			}

			if (h > 0) // Right
			{
				normalizedHorizontalSpeed = horizontalAirVel + 0.5f;
			} 
			else if (h < 0) // Left
			{
				normalizedHorizontalSpeed = horizontalAirVel - 0.5f;
			}
			else
			{
				normalizedHorizontalSpeed = horizontalAirVel;
			}
			
			//Debug.Log(normalizedHorizontalSpeed);
		}

			// Jump data
		if (jump)
		{
			if(squatTimer < .2)
				squatTimer += Time.deltaTime;
			else if(squatTimer >= .2)
			{
				_velocity.y = Mathf.Sqrt(2f * jumpHeight * -gravity);
				jump = false;
				squatTimer = 0;
			}
		}

			// Variables and junk determine movement smoothness
		var smoothedMovementFactor = _controller.isGrounded ? groundDamping : inAirDamping;
		
        	// Final velocity.x is calculated, sprinting is checked as well
		if (sprint && _controller.isGrounded)
			_velocity.x = Mathf.Lerp(_velocity.x, normalizedHorizontalSpeed * sprintSpeed, Time.deltaTime * smoothedMovementFactor);
		else if (State != PlayerState.CROUCH)
			_velocity.x = Mathf.Lerp(_velocity.x, normalizedHorizontalSpeed * walkSpeed, Time.deltaTime * smoothedMovementFactor);
		else
			_velocity.x = Mathf.Lerp(_velocity.x, normalizedHorizontalSpeed * crouchSpeed, Time.deltaTime * smoothedMovementFactor);


			// Gravity is taken care of
		_velocity.y += Time.deltaTime * gravity;

		if (_controller.collisionState.becameGroundedThisFrame && anim.GetBool("Jump") == true)
		{
			anim.SetBool("Jump", false);

			_velocity.Set(0, 0, 0);
		}

			// Finally, velocity is applied to object
		_controller.move( Time.deltaTime * _velocity);

		anim.SetFloat("Speed", Mathf.Abs(_velocity.x));
	}
}