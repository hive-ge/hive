#include "includes.hpp"

using namespace hive;
// Create Global Memory Object.
hive::DataPool hive::general_data_pool(4096);


int main(int arg_ct, char ** args)
{
    hive::DroneDataPool pool(120); // Allocate enough space for 120 data elements of 32 bytes.

    ASSERT(DroneDataPool::byteSize() == 120 * 32 + 120 + 32);

    // Multiple Allocations
    const unsigned allocation_count = 100;

    hive::RenderableProp::Ref refs[allocation_count];

    for (int i = 0; i < allocation_count; i++)
        refs[i] = pool.createObjectReturnRef<hive::RenderableProp>();

    // The references should be valid and have an index identical to their array index location.
    unsigned indexA = refs[allocation_count - 1].getIndex();
    unsigned indexB = refs[allocation_count - 50].getIndex();
    unsigned indexC = refs[0].getIndex();

    ASSERT(indexA == allocation_count - 1);
    ASSERT(indexB == allocation_count - 50);
    ASSERT(indexC == 0);

    // Deallocation of data should free positions
    for (int i = 0; i < allocation_count; i++) pool.deleteObject(refs[i]);

    for (int i = 0; i < allocation_count; i++)
        refs[i] = pool.createObjectReturnRef<hive::RenderableProp>();

    // This reference should be valid and have an index; the current implementation
    // causes the index to be assigned opposite the way they were released
    ASSERT(refs[allocation_count - 1].getIndex() == 0);
    ASSERT(refs[allocation_count - 2].getIndex() == 1);
    ASSERT(refs[allocation_count - 3].getIndex() == 2);
    ASSERT(refs[allocation_count - 4].getIndex() == 3);
    ASSERT(refs[allocation_count - 5].getIndex() == 4);
    ASSERT(refs[allocation_count - 6].getIndex() == 5);

    ASSERT(refs[0].getIndex() == allocation_count - 1);

    try {
        //
        hive::Prop::Ref prop = pool.createObjectReturnRef<hive::Prop>();
        ASSERT("Should not be able to construct root prop objects." && false);

    } catch (int) {
        // All systems good at this point
    }

    hive::ImageProp::Ref prop = pool.createObjectReturnRef<hive::ImageProp>();

    ASSERT(prop);

    hive::ImageProp * prop_ptr = prop.ptr();

    ASSERT(pool.getReference(prop_ptr) == prop);

    std::cout << hive::Prop::global_id << std::endl;

    // Data Field Memory Allocation
    auto char_buffer = general_data_pool.allocate<char[512]>();

    auto & d = char_buffer.raw();

    d[0] = 2;

    char_buffer.update();

    SUCCESS();
}