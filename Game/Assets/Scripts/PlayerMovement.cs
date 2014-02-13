using UnityEngine;
using System.Collections;

public class PlayerMovement : MonoBehaviour 
{
        // movement config
    public float gravity = -15f;
    public float walkSpeed = 8f;
    public float sprintSpeed = 120f;
    public float groundDamping = 20f;
    public float inAirDamping = 5f;
    public float jumpHeight = 3f;
    
    [HideInInspector]
    private float normalizedHorizontalSpeed = 0;
    
    private CharacterController2D _controller;
    private RaycastHit2D _lastControllerColliderHit;
    private Vector3 _velocity;

        // bools
    bool jump = false;
    bool sprint = false;
    
    void Awake()
    {
            // Get the component from the object, contains functions and data used in movement
        _controller = GetComponent<CharacterController2D>();
        _controller.onControllerCollidedEvent += onControllerCollider;
    }
    
    
    void onControllerCollider( RaycastHit2D hit )
    {
        // bail out on plain old ground hits
        if( hit.normal.y == 1f )
            return;
        
        // logs any collider hits if uncommented
        //Debug.Log( "flags: " + _controller.collisionState + ", hit.normal: " + hit.normal );
    }

    void Update()
    {
            // Input
        if (Input.GetButtonDown("Jump") && _controller.isGrounded)
        {
            jump = true;
        }

        sprint = Input.GetButton ("Sprint");
 
        float h = Input.GetAxis("Horizontal");

            // Create local velocity to be used in all further calculations
        _velocity = _controller.velocity;

            // Ground movement
        if (_controller.isGrounded)
        {
            _velocity.y = 0; // On the ground, no need for y
        
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

            // Jump data
        if (jump)
        {
            _velocity.y = Mathf.Sqrt( 2f * jumpHeight * -gravity );

            jump = false;
        }
       
            // Variables and junk determine movement smoothness
        var smoothedMovementFactor = _controller.isGrounded ? groundDamping : inAirDamping;

            // Final velocity.x is calculated, sprinting is checked as well
        if (_controller.isGrounded)
        {
            if (!sprint)
                _velocity.x = Mathf.Lerp( _velocity.x, normalizedHorizontalSpeed * walkSpeed, Time.deltaTime * smoothedMovementFactor );
            else
                _velocity.x = Mathf.Lerp( _velocity.x, normalizedHorizontalSpeed * sprintSpeed, Time.deltaTime * smoothedMovementFactor );
        }

            // Gravity is taken care of
        _velocity.y += Time.deltaTime * gravity;

            // Finally, velocity is applied to object
        _controller.move( Time.deltaTime * _velocity);
    }
}