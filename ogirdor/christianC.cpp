// Name: Christian Capusi
// Date: June 21, 2017
// Purpose: Create a sidescroller with a team

// What I need to do:
//     -Main character level collision

// What I have done:
//     -Main Character Movement
//     -Other Sprite Movement
//     -Main Character Dynamic Jump Physics
//     -Main Character Shoot + Particle Phsyics
//     -Working power ups
//     -Enemy projectile collision and "death"

// Cool features:
//     -Movement is created through an illusion by moving everything but
//         the main character
//     -Particles react to characters movement!
//     -Jump height is dynamically calculated! 
//     -Attributes (health, movementspeed, etc.) are stored in a class
//         this makes it easy to manipulate. Good for power ups!

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "fonts.h"
#include "game.h"

using namespace std;
void moveSpriteRight(Sprite *sprt)
{
    if (gl.camera[0] > 0 && gl.cantMove == false) {
        sprt->cx = sprt->cx + gl.movementSpeed;
    }
}

void moveSpriteLeft(Sprite *sprt)
{
    if (gl.cantMove == false) {
        sprt->cx = sprt->cx - gl.movementSpeed;
    }
}

void moveLevelRight()
{
    if (gl.camera[0] > 0 && gl.cantMove == false) {
        gl.camera[0] -= gl.movementSpeed;
    }
}

void moveLevelLeft()
{
    if (gl.cantMove == false) {
        gl.camera[0] += gl.movementSpeed;
    }
}

void tileCollision(Vec *tile, int row, int col)
{
    //printf("%i\n", lev.dynamicHeight[row]);
    if ((((mainChar.cy) >= (tile->y)) && ((mainChar.cy) <= (tile->y) + lev.tilesize[1]))
        && (((mainChar.cx) >= (tile->x)) && ((mainChar.cx) <= (tile->x) + lev.tilesize[0]))) {
        printf("row: %i - col: %i\n", row, col);
        if (gl.directionFlag == 0) {
            gl.camera[0] -= gl.movementSpeed;
            gl.xc[0] -= 0.001;
            gl.xc[1] -= 0.001;
            moveSpriteRight(&mariEnemy);
            moveSpriteRight(&female);
            moveSpriteRight(&obama);
            moveSpriteRight(&sun);
            moveSpriteRight(&heart1);
            moveSpriteRight(&heart2);
            moveSpriteRight(&shield1);
            moveSpriteRight(&speedboost1);
            moveSpriteRight(&turret);
            moveSpriteRight(&enemy1);
            moveSpriteRight(&godzilla);
        }
        if (gl.directionFlag == 1) {
            gl.camera[0] += gl.movementSpeed;
            gl.xc[0] += 0.001;
            gl.xc[1] += 0.001;
            moveSpriteLeft(&mariEnemy);
            moveSpriteLeft(&female);
            moveSpriteLeft(&obama);
            moveSpriteLeft(&sun);
            moveSpriteLeft(&heart1);
            moveSpriteLeft(&heart2);
            moveSpriteLeft(&shield1);
            moveSpriteLeft(&speedboost1);
            moveSpriteLeft(&turret);
            moveSpriteLeft(&enemy1);
            moveSpriteLeft(&godzilla);
        }
        if (!((((mainChar.cy) >= (tile->y)) && ((mainChar.cy) <= (tile->y - 20) + lev.tilesize[1]))
            && (((mainChar.cx) >= (tile->x - 2)) && ((mainChar.cx) <= (tile->x + 20) + lev.tilesize[0])))) {
                gl.isJumpingFlag = false;
                mainChar.cy = mainChar.cy + 20;
        }
    }
    
    if (!((((mainChar.cy - 3) >= (tile->y)) && ((mainChar.cy - 3) <= (tile->y - 20) + lev.tilesize[1]))
        && (((mainChar.cx) >= (tile->x)) && ((mainChar.cx) <= (tile->x + 20) + lev.tilesize[0])))) {
            gl.initialJumpCy = 85;
    }

}

void spriteDisappear(Sprite* sprt)
{
    sprt->cx = -9999999;
}


void enemyParticleCollision(Particle* p, Sprite *sprt)
{
    if ( p->cx > (sprt->cx - 15.0) && p->cx < (sprt->cx + 15.0)) {
        if (p->cy > (sprt-> cy - 15.0) && p->cy < (sprt->cy + 15)) {
            //when particle hits enemy, teleport to all the way up
            //this doesnt mess up particle off screen detection
            p->cy = 1000;
            sprt->health = sprt->health - 1;
        }
        if (sprt->health == 0) {
            //when enemy reaches 0 health, they "die" by teleporting
            spriteDisappear(sprt);
        }
    }
}

void particleMove()
{
    //move particles based on character movement.
    //if character is moving same direction, slow down
    //if character is moving opposite direction, speed up
    for(int i = 0; i < gl.particleCount; i++) {
        if (gl.keys[XK_Left] && gl.particle[i].direction == 1) {
            gl.particle[i].cx = gl.particle[i].cx + gl.movementSpeed;
        }
        if (gl.keys[XK_Right] && gl.particle[i].direction == 0) {
            gl.particle[i].cx = gl.particle[i].cx - gl.movementSpeed;
        }
    }
}

void particlePhysics(int charSelect)
{
    if (charSelect == 1)
        gl.tempTexture = gl.purpleballTexture;
    if (charSelect == 2)
        gl.tempTexture = gl.moltenballTexture;
    if (charSelect == 3)
        gl.tempTexture = gl.texthighlightTexture;
    if (charSelect == 4)
        gl.tempTexture = gl.americaballTexture;
    if (charSelect == 5)
        gl.tempTexture = gl.americaballTexture;
    if (charSelect == 6)
        gl.tempTexture = gl.americaballTexture;
   
     for (int i = 0; i < gl.particleCount; i++) {
         glColor3ub(255, 255, 255);
         glPushMatrix();
         glTranslatef(gl.particle[i].cx, gl.particle[i].cy, 0);
         float w = gl.particleWidth;
         float h = gl.particleHeight;
         glBindTexture(GL_TEXTURE_2D, gl.tempTexture);
         glBegin(GL_QUADS);
         glTexCoord2f(0.0, 1.0); glVertex2i(-w, -h);
         glTexCoord2f(0.0, 0.0); glVertex2i(-w,  h);
         glTexCoord2f(1.0, 0.0); glVertex2i(w,   h);
         glTexCoord2f(1.0, 1.0); glVertex2i(w,  -h);
         glEnd();
         glPopMatrix();
         if (gl.particle[i].direction == 1) {
             gl.particle[i].cx = gl.particle[i].cx - gl.particle[i].velocity;
         }
         if (gl.particle[i].direction == 0) {
             gl.particle[i].cx = gl.particle[i].cx + gl.particle[i].velocity;
         }
         if (gl.particle[i].cx < 0 || gl.particle[i].cx > gl.xres) {
             gl.particle[i] = gl.particle[gl.particleCount - 1];
             gl.particleCount--;
         }
         //projectilecollisions
         enemyParticleCollision(&gl.particle[i], &enemy1);
         enemyParticleCollision(&gl.particle[i], &turret);
         enemyParticleCollision(&gl.particle[i], &mariEnemy);
     }
}

void makeParticle()
{
    gl.particle[gl.particleCount].cx = mainChar.cx;
    gl.particle[gl.particleCount].cy = mainChar.cy;
    gl.particle[gl.particleCount].velocity = gl.particleVelocity;
    gl.particle[gl.particleCount].direction = gl.directionFlag;
    gl.particleCount++;
}

void shootParticle()
{
    if(gl.oneOffShootFlag == true) {
        printf("Shoot\n");
        makeParticle(); 
        gl.oneOffShootFlag = false;
    }
}

void shootWalkRight(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx+w, cy-h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx-w, cy-h);
    glTexCoord2f(tx,              ty); glVertex2i(cx-w, cy+h);
    glTexCoord2f(tx + .25,        ty); glVertex2i(cx+w, cy+h);
}

void shootWalkLeft(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx-w, cy-h);
    glTexCoord2f(tx + .25,        ty); glVertex2i(cx-w, cy+h);
    glTexCoord2f(tx,              ty); glVertex2i(cx+w, cy+h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx+w, cy-h);
}    

void shootStandRight(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .666); glVertex2i(cx+w, cy-h);
    glTexCoord2f(tx,       ty + .666); glVertex2i(cx-w, cy-h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx-w, cy+h);
    glTexCoord2f(tx+.25,   ty + .333); glVertex2i(cx+w, cy+h);
}

void shootStandLeft(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx,       ty + .666); glVertex2i(cx+w, cy-h);
    glTexCoord2f(tx + .25, ty + .666); glVertex2i(cx-w, cy-h);
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx-w, cy+h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx+w, cy+h);
}

void standRight(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx+w, cy-h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx-w, cy-h);
    glTexCoord2f(tx,              ty); glVertex2i(cx-w, cy+h);
    glTexCoord2f(tx + .25,        ty); glVertex2i(cx+w, cy+h);
}

void standLeft(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25, ty + .333); glVertex2i(cx-w, cy-h);
    glTexCoord2f(tx + .25,        ty); glVertex2i(cx-w, cy+h);
    glTexCoord2f(tx,              ty); glVertex2i(cx+w, cy+h);
    glTexCoord2f(tx,       ty + .333); glVertex2i(cx+w, cy-h);
}

void jumpRight(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx,          ty + 1); glVertex2i(cx-w, cy-h);
    glTexCoord2f(tx,       ty + .666); glVertex2i(cx-w, cy+h);
    glTexCoord2f(tx +.25,  ty + .666); glVertex2i(cx+w, cy+h);
    glTexCoord2f(tx +.25,     ty + 1); glVertex2i(cx+w, cy-h);
}

void jumpLeft(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    glTexCoord2f(tx + .25,    ty + 1); glVertex2i(cx-w, cy-h);
    glTexCoord2f(tx + .25, ty + .666); glVertex2i(cx-w, cy+h);
    glTexCoord2f(tx,       ty + .666); glVertex2i(cx+w, cy+h);
    glTexCoord2f(tx,          ty + 1); glVertex2i(cx+w, cy-h);
}

void jump()
{
    printf("Jump\n");
    if (gl.isJumpingFlag == 0) {
        //temporarily store current y coord
        gl.initialJumpCy = mainChar.cy;
        //temporarily store max jump height
        gl.finalJumpCy = gl.initialJumpCy + gl.jumpHeight;
        gl.isJumpingFlag = 1;
        gl.jumpDirectionFlag = 1;
    }
}

void checkJump()
{
    // if character started jump, increment to max height
    if (gl.isJumpingFlag == 1 && gl.jumpDirectionFlag == 1) {
        if (mainChar.cy <= gl.finalJumpCy) {
            mainChar.cy = 1 + mainChar.cy + ((gl.finalJumpCy - mainChar.cy) * gl.jumpRate);
        } 
        if (mainChar.cy >= gl.finalJumpCy) {
            gl.jumpDirectionFlag = 0;
        }
    }
    // When character reaches max height, decrement to highest tile on x coord
    if (gl.isJumpingFlag == 1 && gl.jumpDirectionFlag == 0) {
        if (mainChar.cy > gl.initialJumpCy) {
        //if (!)
            mainChar.cy = mainChar.cy - ((gl.finalJumpCy - mainChar.cy) * gl.jumpRate) - 1;
        }
        if (mainChar.cy <= gl.initialJumpCy) {
            mainChar.cy = gl.initialJumpCy;
            gl.isJumpingFlag = 0;
        }
    }
}

void conditionalRenders(Flt tx, Flt ty, Flt cx, Flt w, Flt cy, Flt h)
{
    checkJump();
    if (gl.keys[XK_Right] && gl.keys[XK_space] == 0 &&
        gl.isJumpingFlag == 0) {
            shootWalkRight(tx,ty,cx,w,cy,h);
            gl.directionFlag = 0;
    } else if (gl.keys[XK_Left] && gl.keys[XK_space] == 0 &&
        gl.isJumpingFlag == 0) {
            shootWalkLeft(tx,ty,cx,w,cy,h);
            gl.directionFlag = 1;
    } else if ((gl.keys[XK_space] && gl.directionFlag == 0 &&
        gl.isJumpingFlag == 0) || (gl.keys[XK_space] && gl.keys[XK_Right])) {
            shootStandRight(tx,ty,cx,w,cy,h);
            shootParticle();
    } else if ((gl.keys[XK_space] && gl.directionFlag == 1 &&
        gl.isJumpingFlag == 0) || (gl.keys[XK_space] && gl.keys[XK_Left])) {
            shootStandLeft(tx,ty,cx,w,cy,h);
            shootParticle();
    } else if (gl.keys[XK_Left] && gl.isJumpingFlag == 1 &&
        (gl.keys[XK_space] == 1 || gl.keys[XK_space] == 0)) {
            jumpLeft(tx, ty, cx, w, cy, h);
            gl.directionFlag = 1;
    } else if (gl.keys[XK_Right] && gl.isJumpingFlag == 1 &&
        (gl.keys[XK_space] == 1 || gl.keys[XK_space] == 0)) {
            jumpRight(tx, ty, cx, w, cy, h);
            gl.directionFlag = 0;
    }

    if (gl.keys[XK_Right] == 1 || gl.keys[XK_Left] == 1) {
        particleMove();
    }
    if (gl.keys[XK_space] == 0) {
        gl.oneOffShootFlag = true;
    }
    if (gl.keys[XK_Up] && gl.isJumpingFlag == 0) {
        jump();
    }
    if (gl.keys[XK_Left] == 0 && gl.keys[XK_Right] == 0 &&
        gl.directionFlag == 1 && gl.keys[XK_space] == 0 &&
        gl.isJumpingFlag == 0) {
            standLeft(tx, ty, cx, w, cy, h);
    }
    if (gl.keys[XK_Right] == 0 && gl.keys[XK_Right] == 0 &&
        gl.directionFlag == 0 && gl.keys[XK_space] == 0  &&
        gl.isJumpingFlag == 0) {
            standRight(tx, ty, cx, w, cy, h);
    }
    if (gl.keys[XK_Left] == 0 && gl.keys[XK_Right] == 0 &&
        gl.directionFlag == 1 && (gl.keys[XK_space] == 0 || 
        gl.keys[XK_space] == 1) && gl.isJumpingFlag == 1) {
            jumpLeft(tx, ty, cx, w, cy, h);
	    if (gl.keys[XK_space] == 1) {
	        shootParticle();
            }
    }
    if (gl.keys[XK_Right] == 0 && gl.keys[XK_Right] == 0 &&
        gl.directionFlag == 0 && (gl.keys[XK_space] == 0 ||
        gl.keys[XK_space] == 1) && gl.isJumpingFlag == 1) {
            jumpRight(tx, ty, cx, w, cy, h);
            if (gl.keys[XK_space] == 1) {
                shootParticle();
            }
    }
}

void clearScreen()
{
    glClearColor(0.1,0.1,0.1,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}


void renderMainCharacter(int charSelect)
{
    if (charSelect == 1)
        gl.tempMainCharacterTexture = gl.maincharacter1Texture;
    if (charSelect == 2)
        gl.tempMainCharacterTexture = gl.maincharacter2Texture;
    if (charSelect == 3)
        gl.tempMainCharacterTexture = gl.maincharacter3Texture;
    if (charSelect == 4)
        gl.tempMainCharacterTexture = gl.maincharacter4Texture;
    if (charSelect == 5)
        gl.tempMainCharacterTexture = gl.maincharacter4Texture;
    if (charSelect == 6)
        gl.tempMainCharacterTexture = gl.maincharacter4Texture;

    mainChar.cx = gl.xres/2.0;
    float h = 25.0;
    float w = h * .903;
    glPushMatrix();
    //glTranslated(mainChar.pos[0], mainChar.pos[1], 0);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, gl.tempMainCharacterTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    int ix = gl.maincharacterFrame % 4;
    int iy = 3;
    if (gl.maincharacterFrame >= 4 && gl.attackFlag == 0)
        iy = 0;
    float tx = (float)ix / 4.0;
    float ty = (float)iy / 3.0;
    glBegin(GL_QUADS);
    //works best with
    //conditionalRenders(tx, ty, -16, w, 16, h);
    conditionalRenders(tx, ty, mainChar.cx, w, mainChar.cy, h);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void renderSpeedboost1()
{
    if (gl.speedboost1Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.speedboostTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(speedboost1.cx-w, speedboost1.cy-h);
        glTexCoord2f(tx + 1,     ty); glVertex2i(speedboost1.cx-w, speedboost1.cy+h);
        glTexCoord2f(tx,        ty ); glVertex2i(speedboost1.cx+w, speedboost1.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(speedboost1.cx+w, speedboost1.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        if ((mainChar.cx > (speedboost1.cx - 10) && mainChar.cx < (speedboost1.cx + 10))
        && ((mainChar.cy > (speedboost1.cy - 10)) && (mainChar.cy < (speedboost1.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            printf("Picked up speed boost! Movement Speed: + 0.2!\n");
            gl.speedboost1Flag = false;
            spriteDisappear(&speedboost1);
            gl.movementSpeed = gl.movementSpeed + 0.2;
        }
    }
}

void renderShield1()
{
    if (gl.shield1Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.shieldTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(shield1.cx-w, shield1.cy-h);
        glTexCoord2f(tx + 1, ty    ); glVertex2i(shield1.cx-w, shield1.cy+h);
        glTexCoord2f(tx,     ty    ); glVertex2i(shield1.cx+w, shield1.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(shield1.cx+w, shield1.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        if ((mainChar.cx > (shield1.cx - 10) && mainChar.cx < (shield1.cx + 10))
        && ((mainChar.cy > (shield1.cy - 10)) && (mainChar.cy < (shield1.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            printf("Picked up Shield!\n");
            gl.shield1Flag = false;
            spriteDisappear(&shield1);
        }
    }
}

void renderHeart1()
{
    if (gl.heart1Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.heartaddTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(heart1.cx-w, heart1.cy-h);
        glTexCoord2f(tx + 1, ty    ); glVertex2i(heart1.cx-w, heart1.cy+h);
        glTexCoord2f(tx,     ty    ); glVertex2i(heart1.cx+w, heart1.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(heart1.cx+w, heart1.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        if ((mainChar.cx > (heart1.cx - 10) && mainChar.cx < (heart1.cx + 10))
        && ((mainChar.cy > (heart1.cy - 10)) && (mainChar.cy < (heart1.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            gl.heart1Flag = false;
            spriteDisappear(&heart1);
            printf("Picked up Health Pack! Health + 5\n");
            if (mainChar.health < 30) {
                mainChar.health = mainChar.health + 5;
                if (mainChar.health > 30)
                    mainChar.health = 30;
            }
        }
    }
}

void renderHeart2()
{
    if (gl.heart2Flag != false) {
        float h = 25;
        float w = 25;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.heartaddTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
        int ix = 1, iy = 1;
        float tx = (float)ix;
        float ty = (float)iy;
        glBegin(GL_QUADS);
        glTexCoord2f(tx + 1, ty + 1); glVertex2i(heart2.cx-w, heart2.cy-h);
        glTexCoord2f(tx + 1, ty    ); glVertex2i(heart2.cx-w, heart2.cy+h);
        glTexCoord2f(tx,     ty    ); glVertex2i(heart2.cx+w, heart2.cy+h);
        glTexCoord2f(tx,     ty + 1); glVertex2i(heart2.cx+w, heart2.cy-h);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        if ((mainChar.cx > (heart2.cx - 10) && mainChar.cx < (heart2.cx + 10))
        && ((mainChar.cy > (heart2.cy - 10)) && (mainChar.cy < (heart2.cy + 10)))) {
            //if character picks up power up,
            //stop rendering and set x to -999999 to avoid
            //picking up invisible power ups
            gl.heart2Flag = false;
            printf("Picked up Health Pack! Health + 5\n");
            spriteDisappear(&heart2);
            if (mainChar.health < 30) {
                mainChar.health = mainChar.health + 5;
                if (mainChar.health > 30)
                    mainChar.health = 30;
            }
        }
    }
}

void christianInit()
{
    //initialize my sprites' x and y positions
    mainChar.cy = 85;
    shield1.cx = 900;
    shield1.cy = 180;
    heart1.cx = 700;
    heart1.cy = 90;
    heart2.cx = 800;
    heart2.cy = 90;
    speedboost1.cx= 750;
    speedboost1.cy = 90;
}
void renderChristianSprites(int charSelect)
{
    printf("x: %f - y: %f\n", mainChar.cx, mainChar.cy);
    renderMainCharacter(charSelect);
    renderShield1();
    renderSpeedboost1();
    renderHeart1();
    renderHeart2();
    particlePhysics(charSelect);
}
