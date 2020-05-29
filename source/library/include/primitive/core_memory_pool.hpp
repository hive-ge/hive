#pragma once

#include "include/primitive/core_string_hash.hpp"
#include "include/primitive/core_typedef.hpp"
#include <cstdlib>

namespace hive
{
    constexpr unsigned MAX_ELEMENT_SIZE = 1 << 24;
    /**
      Allocates large blocks of contiguous memory.
  */
    struct LargeBlockAllocator {
        static void * allocate(size_t size) { return malloc(size); };
        static void deallocate(void * ptr) { free(ptr); }
    };

    template <class Allocator> struct DroneDataHandle_ {

        constexpr static unsigned MAX_INDEX = (1 << 24) - 1;
        /**
         * Type information stored in high 8 bits.
         * Index information stored in low 24 bits;
         */
      private:
        unsigned index_type = 0;

        unsigned unique_id;


      public:
        inline unsigned getIndex() { return index_type & 0xFFFFFF; }

        inline unsigned getType() { return index_type >> 24; }

        inline unsigned getID() { return unique_id; }
        /**
         * ind : The index location in the ObjectBuffer for this object.
         * type : Type of object this handle references
         * id: The unique of the object that this references.
         */
        DroneDataHandle_(unsigned ind, unsigned type, unsigned id) : unique_id(id)
        {
            index_type = type << 24 | ind;
        }

        DroneDataHandle_(unsigned ind) : unique_id(0) { index_type = 0; }

        DroneDataHandle_(const DroneDataHandle_ & hndl)
            : index_type(hndl.index_type), unique_id(hndl.unique_id)
        {
        }

        ~DroneDataHandle_() { Allocator::incrementIndexedReference(getIndex()); }

        template <class T> T * operator[](StringHash64 tag)
        {
            if (is<Drone>()) {
            }
            return nullptr;
        };

        template <class T> constexpr bool is() const
        {
            if (T::DroneDataType == getType()) return true;

            return false;
        }

        template <class T> T & operator*() const
        {
            if (!is()) throw "failed Derefence of a nullptr";
            return *Allocator::retrieveIndexedPointer(getIndex());
        }

        template <class T> T * operator->()
        {
            if (!is()) return nullptr;
            return Allocator::retrieveIndexedPointer(getIndex());
        }
    };


    /**
     * Storage to use for dynamic objects,
     * memory relocation, and small object pointers (32bit);
     */
    template <const int object_size> struct ObjectMemPool {

        constexpr static unsigned DroneDataStructSize     = object_size;
        constexpr static unsigned DroneDataNullObjectType = 0xFF;
        typedef DroneDataHandle_<ObjectMemPool<object_size>> ObjectPointer;

        static void * min_object_location;
        static void * max_object_location;

      private:
        typedef unsigned lu_element;

        // TABLE FOR STORING
        // Number of elements;
        static struct Data {
            unsigned element_count;
            unsigned byte_size;
            unsigned next_free;
            unsigned current_uuid;
            ObjectPointer * LU;
            char * pool;
        } * data;

      public:
        constexpr ObjectMemPool(size_t element_amount)
        {
            if (element_amount < MAX_ELEMENT_SIZE) {

                const unsigned construct_size =
                    sizeof(Data) + element_amount + element_amount * object_size;

                data = LargeBlockAllocator::allocate(construct_size);

                if (!data) return;

                Data & data_ref        = data;
                data_ref.element_count = element_amount;
                data_ref.byte_size     = construct_size;
                data_ref.next_free     = 0;
                data_ref.current_uuid  = 0;
                data_ref.LU            = static_cast<char *>(data) + sizeof(Data);
                data_ref.pool =
                    static_cast<char *>(data) + sizeof(Data) + sizeof(lu_element) * element_amount;

                // Initialize LU list
                // Using the ObjectPointer with the Null Id, the index of this handl
                // will point to the next free index slot available.
                for (int i = 0; i < element_amount - 1; i++)
                    data_ref.LU[i] = ObjectPointer(i + 1, DroneDataNullObjectType, 0);

                // The last free object will always have an index greater than the available
                // objects.
                data_ref.LU[element_amount - 1] =
                    ObjectPointer(element_amount, DroneDataNullObjectType, 0);

                min_object_location = data_ref.pool;
                max_object_location = data_ref.pool + ((element_amount - 1) * object_size);
            }
        }

        ~ObjectMemPool()
        {
            if (data) LargeBlockAllocator::deallocate(data);
        }

        template <class T> static T * createObject()
        {
            static_assert(T::DroneDataType >= 0 && T::DroneDataType < 255,
                          "Should Have DroneDataType Type");

            if (data->next_free < data->element_count) {

                const unsigned index = data->next_free;

                ObjectPointer data_slot = data->LU[data->next_free];

                data->next_free = data_slot.getIndex();

                T * obj = new (data->pool[index * object_size]) T();

                data->LU[index] = ObjectPointer(index, T::DroneDataType, data->current_uuid++);

                return obj;
            }

            return nullptr;
        };

        static void deleteObject(void * ptr)
        {
            if (ptr >= min_object_location && ptr <= max_object_location &&
                (unsigned long long)ptr % object_size == 0) {
                unsigned index =
                    (unsigned)(((unsigned long long)min_object_location - (unsigned long long)ptr) /
                               object_size);

                data->LU[index] = ObjectPointer(data->next_free, DroneDataNullObjectType, 0);

                data->next_free = index;
            }
        };

        static void incrementIndexedReference(unsigned i){};
        static void decrementIndexedReference(unsigned i){};
        static void * retrieveIndexedPointer(unsigned index)
        {
            return data->pool[index * object_size];
        }

        void destroyObject(ObjectPointer object) { data->generation++; };

        size_t byteSize()
        {

            if () return data->byte_size;
            return 0;
        }

        bool resize() {}
    };


    typedef ObjectMemPool<32> DroneDataPool;
    typedef DroneDataPool::ObjectPointer DroneDataHandle;
} // namespace hive