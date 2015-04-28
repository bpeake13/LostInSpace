The most notable files in our source are:

UPawnPlanatoidMovementMode and all its child classes. These classes are the core of how the gravitational pawn is able to move and
describe different movement modes for the player.

UWanderPath, UAttackPlayerPlanet, and UCanSeePlayerPlanet. These are our current AI for the enemy ship and describe how it should move
and act. These are also closely related to ABaseEnemyPawn.

AProjectile is a base class for all projectiles in our game.