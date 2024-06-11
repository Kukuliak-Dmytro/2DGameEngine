


//Class meant for collision
class ColliderComponent : public Component {
public:
    //COllider rectangle
    SDL_Rect collider;
    //A pointer to <TransformComponent> class. It would be initialized later
    TransformComponent* transform; 
    //Default constructor
    ColliderComponent() = default;
    ColliderComponent( int x, int y) {
    
        collider.x = x;
        collider.y = y;
        collider.w = collider.h = 32;
    }
    ColliderComponent( int x, int y, int w, int h) {
        collider.x = x;
        collider.y = y;
        collider.w = w;
        collider.h = h;
    }

    void init() override {
        //If entity doesn`t already have transform component, we add it
        if (!entity->hasComponent<TransformComponent>()) {
            entity->addComponent<TransformComponent>();
        }
        //Assign the transform component the reference, to avoid copying 
        //And all the changes we make here would affect the <TransformComponent> class
        transform = &entity->getComponent<TransformComponent>();
       
    }

    void update() override {
        //The collider is dependent of the position
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width;
        collider.h = transform->height;
    }
};

