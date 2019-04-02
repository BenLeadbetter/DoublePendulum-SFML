// This program will simulate a double pendulum using Euler's method
// to model the motion from the lagrangian.
//
// The velocity is damped frame-to-frame in order to avoid the 
// gradual increase in the system's energy due to the imperfections
// in the approximation.


#include "SFML/Graphics.hpp"
#include <cmath>

using namespace sf;

// Two functions for converting from degrees to radians and back
float degtorad(float angle)
{
    return angle * 3.14159f / 180.0;
}

float radtodeg(float angle)

{
    return angle * 180.0 / 3.14159f;
}

int main()
{
    // Create a video mode object
    VideoMode vm(1000, 800);

    // Create and open a window
    RenderWindow window(vm, "Double Pendulum", Style::Default);
    window.setFramerateLimit(100);

    // Create a variable to control time
    Clock clock;

    // Set max number of iterations per second
    const float MAX_LOOP_RATE = 1000;


    // Create the background: a white rectangle
    RectangleShape background;
    background.setSize(Vector2f(1000, 800));
    background.setFillColor(Color::White);
    background.setOrigin(0,0);
    background.setPosition(0,0);

    // Create texture for a pendulum 
    //***Why are the x/y coordinates oriented accross/up rather than across/down?????*********
    RenderTexture texture_pendulum;
    if(!texture_pendulum.create(88, 229))
        return -1;
    // Draw the string onto the texture
    RectangleShape pendulum_string;
    pendulum_string.setSize((Vector2f(4,145)));
    pendulum_string.setFillColor(Color::Black);
    pendulum_string.setOrigin(2, 0);
    pendulum_string.setPosition(44, 84);
    texture_pendulum.draw(pendulum_string);    
    // Draw the bob onto the texture
    CircleShape pendulum_bob;
    pendulum_bob.setRadius(40);
    pendulum_bob.setFillColor(Color::White);
    pendulum_bob.setOutlineColor(Color::Black);
    pendulum_bob.setOutlineThickness(4);
    pendulum_bob.setOrigin(0, 0);
    pendulum_bob.setPosition(4, 4);
    texture_pendulum.draw(pendulum_bob);
    // Attach the texture to two sprites
    Sprite sprite_pendulum1;
    Sprite sprite_pendulum2;
    sprite_pendulum1.setTexture(texture_pendulum.getTexture());
    sprite_pendulum2.setTexture(texture_pendulum.getTexture());
    sprite_pendulum1.setOrigin(44, 0);
    sprite_pendulum2.setOrigin(44, 0);
    // Place sprites initially in vertical, resting position
    sprite_pendulum1.setPosition(500, 280);
    sprite_pendulum2.setPosition(500, 465);
    sprite_pendulum1.setRotation(0);
    sprite_pendulum2.setRotation(0);

    // Define a couple of physical constants
    const float GRAVITY = 9.81;
    const float RADIUS_PIXELS = 185.0f;
    const float RADIUS = RADIUS_PIXELS / 50.0f;

    // Set up the initial values of the variables
    const float INIT_PHI = 2.7f;
    const float INIT_PSI = 2.2f;
    const float INIT_PHI_DOT = 0.0f;
    const float INIT_PSI_DOT = 0.0f;

    // Set up the physical variables for the pendulum
    float phi = INIT_PHI;
    float psi = INIT_PSI;
    float phi_dot = INIT_PHI_DOT;
    float psi_dot = INIT_PSI_DOT;
    float phi_dot_dot = 0.0f;
    float psi_dot_dot = 0.0f;
    float new_phi = 0.0f;
    float new_psi = 0.0f;

    //Set up main loop
    while(window.isOpen())
    {
        /*
        ***********************************************
        Handle user input here
        ***********************************************
        */

       Event event;

       while(window.pollEvent(event))
       {
            
            if(event.type == Event::Closed)
                window.close();
       }

        


        /*
        ***********************************************
        Update the scene here
        ***********************************************
        */

        // measure time
        Time dt = clock.restart();

        // Update the double pendulum variables
        // Update angles
        new_phi = phi + phi_dot * dt.asSeconds();
        new_psi = psi + psi_dot * dt.asSeconds();

        // Update angular velocities
        // First get values for phi_dot_dot and psi_dot_dot
        phi_dot_dot = ( phi_dot * phi_dot * std::sin(phi - psi) * std::cos(phi - psi)
                        - (GRAVITY / RADIUS) * std::sin(psi) * std::cos(phi - psi)
                        + psi_dot * psi_dot * std::sin(phi-psi)
                        + 2 * (GRAVITY / RADIUS) * std::sin(phi)
                        ) / (std::cos(phi-psi) * std::cos(phi - psi) - 2);

        psi_dot_dot = (
                            - 2 * (GRAVITY / RADIUS) * std::cos(phi - psi) * std::sin(phi)
                            - 2 * phi_dot * phi_dot * std::sin(phi - psi)
                            - psi_dot * psi_dot * std::cos(phi - psi) * std::sin(phi - psi)
                            + 2 * (GRAVITY / RADIUS) * std::sin(psi)
                        ) / (std::cos(phi-psi) * std::cos(phi - psi) - 2);
        
        // Then update values of phi_dot and psi_dot
        phi_dot = (phi_dot + phi_dot_dot * dt.asSeconds()) * 0.9995;
        psi_dot = (psi_dot + psi_dot_dot * dt.asSeconds()) * 0.9995;
        
        //redifine phi and psi with their new values
        phi = new_phi;
        psi = new_psi;

        // Set the pendulum at the position dictated by the variables
        sprite_pendulum1.setRotation(radtodeg(phi));
        sprite_pendulum2.setPosition(-1 * RADIUS_PIXELS * std::sin(phi) + 500, -1 * RADIUS_PIXELS *(1 - std::cos(phi)) + 465);
        sprite_pendulum2.setRotation(radtodeg(psi));



        

        /*
        ***********************************************
        Draw the scene here
        ***********************************************
        */

        
        //clear everything from the last frame
        window.clear();

        // Draw the background
        window.draw(background);

        // Draw the double pendulum
        window.draw(sprite_pendulum2);
        window.draw(sprite_pendulum1);


        // Show everything we just drew
        window.display();

        //Delay the loop renewing to save computer power
        float iteration_time = (clock.getElapsedTime()).asSeconds();
        while(iteration_time < 1 / MAX_LOOP_RATE)
        {
            iteration_time = (clock.getElapsedTime()).asSeconds();
        }

    } // End main loop

    return 0;
}