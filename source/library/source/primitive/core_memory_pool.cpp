#include <hive.h>

namespace hive
{
    template <> DroneDataPool::Data * DroneDataPool::data = nullptr;
    template <> void * DroneDataPool::min_object_location = nullptr;
    template <> void * DroneDataPool::max_object_location = nullptr;
    template <> unsigned DroneDataPool::offset_alignment  = 0;
} // namespace hive