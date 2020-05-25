#include "include/hive.h"

#include <vector>

namespace hive
{

    RenderBoss::RenderBoss()
        : Boss(IDENTIFIER){

          };

    RenderBoss::~RenderBoss(){

    };

    void RenderBoss::setup(){

    };

    void RenderBoss::update(float)
    {
        // Check the drones for update render settings.
        for (auto & drone : drones) {

            auto * prop = drone->props;

            while (prop != nullptr) {

                if (prop->type == StringHash64("PROP_RENDER")) {

                    auto * render_state_prop = static_cast<RenderableProp *>(prop);

                    if (render_state_prop->ALLOW_RENDER()) {
                        std::cout << "Allowing render" << std::endl;
                    }
                }

                prop = prop->next;
            }
        }
    };

    void RenderBoss::teardown(){

    };

} // namespace hive