#include <cmath>

#include "raylib.h"

#define flockSize 150

// height of the window
int screenWidth = 1104;
int screenHeight = 621;
// boid
struct boid {
    Vector2 boidPosition;
    Vector2 boidVelocity;
    Vector2 boidAcceleration;
    float maxSpeed = 5.0f;
    float maxForce = 0.2f;
    int radius = 5;
    float radiusOfSightalign = 50;
    float radiusOfSightcohesion = 30;
    float radiusOfSightseperation = 30;
    // doesent just vanish out of the screentype of shit
    void dontDissapearInTheEdgeBoidFFS() {
        if (boidPosition.x > screenWidth) {
            boidPosition.x = 0;
        } else if (boidPosition.x < 0) {
            boidPosition.x = screenWidth;
        };
        if (boidPosition.y > screenHeight) {
            boidPosition.y = 0;
        } else if (boidPosition.y < 0) {
            boidPosition.y = screenHeight;
        };
    };

    // updates boids and shit
    void updateBoid() {
        boidPosition.x += boidVelocity.x;
        boidPosition.y += boidVelocity.y;
        boidVelocity.x += boidAcceleration.x;
        boidVelocity.y += boidAcceleration.y;
        boidVelocity.x = (boidVelocity.x > maxSpeed) ? maxSpeed : boidVelocity.x;
        boidVelocity.y = (boidVelocity.y > maxSpeed) ? maxSpeed : boidVelocity.y;
        boidAcceleration.x = 0;
        boidAcceleration.y = 0;
    };

    // boid align
    Vector2 alignBoid(boid boids[]) {
        Vector2 steering = {0.0f, 0.0f};
        int totalBoidsInRadius = 0;

        for (int i = 0; i < flockSize; i++) {
            float distance = sqrt(((boidPosition.x - boids[i].boidPosition.x) * (boidPosition.x - boids[i].boidPosition.x)) +
                                  ((boidPosition.y - boids[i].boidPosition.y) * (boidPosition.y - boids[i].boidPosition.y)));
            if (&boids[i] != this && distance < radiusOfSightalign) {
                steering.x += boids[i].boidVelocity.x;
                steering.y += boids[i].boidVelocity.y;
                totalBoidsInRadius++;
            };
        };
        if (totalBoidsInRadius > 0) {
            steering.x /= totalBoidsInRadius;
            steering.y /= totalBoidsInRadius;
            float magnitude = std::sqrt(steering.x * steering.x + steering.y * steering.y);
            steering.x /= magnitude;
            steering.y /= magnitude;
            steering.x *= maxSpeed;
            steering.y *= maxSpeed;
            steering.x -= boidVelocity.x;
            steering.y -= boidVelocity.y;

            float steeringMagnitude = std::sqrt(steering.x * steering.x + steering.y * steering.y);
            float limitFactor = maxForce / steeringMagnitude;
            steering.x *= limitFactor;
            steering.y *= limitFactor;
        };
        return steering;
    };

    Vector2 cohesionBoid(boid boids[]) {
        Vector2 steering = {0.0f, 0.0f};
        int totalBoidsInRadius = 0;

        for (int i = 0; i < flockSize; i++) {
            float distance = sqrt(((boidPosition.x - boids[i].boidPosition.x) * (boidPosition.x - boids[i].boidPosition.x)) +
                                  ((boidPosition.y - boids[i].boidPosition.y) * (boidPosition.y - boids[i].boidPosition.y)));
            if (&boids[i] != this && distance < radiusOfSightcohesion) {
                steering.x += boids[i].boidPosition.x;
                steering.y += boids[i].boidPosition.y;
                totalBoidsInRadius++;
            };
        };
        if (totalBoidsInRadius > 0) {
            steering.x /= totalBoidsInRadius;
            steering.y /= totalBoidsInRadius;
            steering.x -= boidPosition.x;
            steering.y -= boidPosition.y;
            float magnitude = std::sqrt(steering.x * steering.x + steering.y * steering.y);
            steering.x /= magnitude;
            steering.y /= magnitude;
            steering.x *= maxSpeed;
            steering.y *= maxSpeed;
            steering.x -= boidVelocity.x;
            steering.y -= boidVelocity.y;

            float steeringMagnitude = std::sqrt(steering.x * steering.x + steering.y * steering.y);
            float limitFactor = maxForce / steeringMagnitude;
            steering.x *= limitFactor;
            steering.y *= limitFactor;
        };
        return steering;
    };
    Vector2 seperationBoid(boid boids[]) {
        Vector2 steering = {0.0f, 0.0f};
        int totalBoidsInRadius = 0;

        for (int i = 0; i < flockSize; i++) {
            float distance = sqrt(((boidPosition.x - boids[i].boidPosition.x) * (boidPosition.x - boids[i].boidPosition.x)) +
                                  ((boidPosition.y - boids[i].boidPosition.y) * (boidPosition.y - boids[i].boidPosition.y)));
            if (&boids[i] != this && distance < radiusOfSightseperation) {
                Vector2 diff = {(boidPosition.x - boids[i].boidPosition.x) / distance * distance, (boidPosition.y - boids[i].boidPosition.y) / distance * distance};
                steering.x += diff.x;
                steering.y += diff.y;
                totalBoidsInRadius++;
            };
        };
        if (totalBoidsInRadius > 0) {
            steering.x /= totalBoidsInRadius;
            steering.y /= totalBoidsInRadius;
            float magnitude = std::sqrt(steering.x * steering.x + steering.y * steering.y);
            steering.x /= magnitude;
            steering.y /= magnitude;
            steering.x *= maxSpeed;
            steering.y *= maxSpeed;
            steering.x -= boidVelocity.x;
            steering.y -= boidVelocity.y;

            float steeringMagnitude = std::sqrt(steering.x * steering.x + steering.y * steering.y);
            float limitFactor = maxForce / steeringMagnitude;
            steering.x *= limitFactor;
            steering.y *= limitFactor;
        };
        return steering;
    };
    // flock allignment
    void flock(boid boids1[]) {
        Vector2 allignment = alignBoid(boids1);
        Vector2 cohesion = cohesionBoid(boids1);
        Vector2 seperation = seperationBoid(boids1);
        boidAcceleration.x += allignment.x;
        boidAcceleration.y += allignment.y;
        boidAcceleration.x += cohesion.x;
        boidAcceleration.y += cohesion.y;
        boidAcceleration.x += seperation.x;
        boidAcceleration.y += seperation.y;
    };
    // drawing the boids
    void drawBoid() { DrawCircle(boidPosition.x, boidPosition.y, radius, BLUE); }
};

// main void
int main(void) {
    // Initialization
    InitWindow(screenWidth, screenHeight, "boid");
    SetTargetFPS(60);  // fps cap

    // initialisizes the size of our boid flock
    boid boidFlock[flockSize];

    // gives each boid random velocity and randomizes position on screen
    for (int i = 0; i < flockSize; i++) {
        boidFlock[i].boidVelocity = {(float)GetRandomValue(0, 5), (float)GetRandomValue(0, 5)};
        boidFlock[i].boidPosition = {(float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight)};
    };

    // game loop
    while (!WindowShouldClose()) {
        // Update
        // do boid shit
        for (int i = 0; i < flockSize; i++) {
            boidFlock[i].dontDissapearInTheEdgeBoidFFS();
            boidFlock[i].flock(boidFlock);
            boidFlock[i].updateBoid();
        };

        // draw part babyyyyy
        BeginDrawing();

        ClearBackground(BLACK);  // color of background

        // drawing flock of boids
        for (int i = 0; i < flockSize; i++) {
            boidFlock[i].drawBoid();
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();  // Close window and OpenGL context

    // the return statement babyyy
    return 0;
}