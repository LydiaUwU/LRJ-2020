#ifndef LRJ_2020_PLAYER_H
#define LRJ_2020_PLAYER_H


class Player {
private:
    const float radius, speed, rotSpeed, targetOffset, rotRadius;
    const Vector3 cameraOffset;
    float rotPos;

public:
    Player();

    void Update();
    void Draw();
};


#endif //LRJ_2020_PLAYER_H
