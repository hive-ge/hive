#pragma once

#include "include/primitive/core_string_hash.hpp"
#include "include/primitive/core_typedef.hpp"
#include <cstdlib>
#include <new>

namespace hive
{
    constexpr unsigned MAX_ELEMENT_SIZE = (1 << 24) - 2;
    /**
      Allocates large blocks of contiguous memory.
  */
    struct LargeBlockAllocator {
        static void * allocate(size_t size) { return malloc(size); };
        static void deallocate(void * ptr) { free(ptr); }
    };

    template <class Allocator, class Type = void *> struct DroneDataHandle_ {

        enum vTYPE : unsigned { UNDEFINED = 0 };

        constexpr static unsigned MAX_INDEX = (1 << 24) - 1;
        /**
         * Type information stored in high 8 bits.
         * Index information stored in low 24 bits;
         */
      protected:
        unsigned index_type = 0;

        unsigned unique_id;


      public:
        static inline unsigned getIndexTypeCombo(unsigned index, unsigned type)
        {
            return (type << 24) | index;
        }

        inline unsigned getIndex() const { return index_type & 0xFFFFFF; }

        inline unsigned getType() const { return index_type >> 24; }

        inline unsigned getID() const { return unique_id; }
        /**
         * ind : The index location in the ObjectBuffer for this object.
         * type : Type of object this handle references
         * id: The unique of the object that this references.
         */
        DroneDataHandle_(unsigned ind, unsigned type, unsigned id) : unique_id(id)
        {
            index_type = getIndexTypeCombo(ind, type);
        }

        DroneDataHandle_(unsigned ind) : unique_id(0) { index_type = 0; }

        DroneDataHandle_() : index_type(0), unique_id(0) {}

        template <class T>
        DroneDataHandle_(const DroneDataHandle_<Allocator, T> & hndl)
            : DroneDataHandle_(hndl.getIndex(), hndl.getType(), hndl.getID())
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

        constexpr bool operator==(const DroneDataHandle_ t)
        {
            return index_type == t.index_type && t.unique_id == unique_id;
        }

        constexpr bool operator!=(const DroneDataHandle_ t)
        {
            return index_type != t.index_type || t.unique_id != unique_id;
        }


        constexpr bool operator==(const DroneDataHandle_::vTYPE t)
        {
            return (index_type | unique_id) == t;
        }

        constexpr bool operator!=(const DroneDataHandle_::vTYPE t)
        {
            return (index_type | unique_id) != t;
        }

        explicit operator bool() { return (index_type | unique_id) != DroneDataHandle_::UNDEFINED; }

        Type & operator*() const { return *ptr(); }

        Type * operator->() { return ptr(); }

        Type * ptr() { return static_cast<Type *>(Allocator::retrieveIndexedPointer(getIndex())); }

        template <class T> DroneDataHandle_<Allocator, T> reinterpret()
        {
            return static_cast<DroneDataHandle_<Allocator, T>>(*this);
        }

        template <class T> DroneDataHandle_<Allocator, T> assert_reinterpret()
        {
            if (!is<T>()) throw "Handle does is not a referenc to this object.";

            return reinterpret_cast<DroneDataHandle_<Allocator, T>>(*this);
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
        static unsigned offset_alignment;

      private:
        typedef ObjectPointer lu_element;

        // TABLE FOR STORING Object Pool data
        static struct Data {
            unsigned element_count;
            unsigned byte_size;
            unsigned next_free;
            unsigned current_uuid;
            ObjectPointer * LU;
            char * pool;
        } * data;

      public:
        constexpr ObjectMemPool()
        {
            if (!data) {
                std::cout
                    << "ObjectMemPool:: Cannot allocate from empty pool! Use constructor with  "
                       "ObjectMemPool(PoolSize) to "
                       "initialize poll data."
                    << std::endl;
                throw 0;
            }
        }

        constexpr ObjectMemPool(size_t element_amount)
        {
            if (!data && element_amount < MAX_ELEMENT_SIZE) {

                const unsigned construct_size =
                    sizeof(Data) + element_amount + element_amount * object_size;

                data = static_cast<Data *>(LargeBlockAllocator::allocate(construct_size));

                if (!data) {
                    std::cout << "ObjectMemPool:: unable to allocate memory for amount request: "
                              << element_amount << std::endl;
                    throw 0;
                }

                Data & data_ref        = *data;
                data_ref.element_count = element_amount;
                data_ref.byte_size     = construct_size;
                data_ref.next_free     = 0;
                data_ref.current_uuid  = 0;
                data_ref.LU   = reinterpret_cast<ObjectPointer *>(reinterpret_cast<char *>(data) +
                                                                sizeof(Data));
                data_ref.pool = reinterpret_cast<char *>(data) + sizeof(Data) +
                                sizeof(lu_element) * element_amount;


                offset_alignment = (unsigned long long)data_ref.pool % object_size;

                // Initialize LU list
                // Using the ObjectPointer with the Null Id, the index of this handle
                // will point to the next free index slot available.
                for (int i = 0; i < element_amount - 1; i++) {
                    data_ref.LU[i]     = ObjectPointer(i + 1, DroneDataNullObjectType, 0);
                    ObjectPointer test = data_ref.LU[i];
                }

                // The last free object will always have an index greater than the available
                // objects.
                data_ref.LU[element_amount - 1] =
                    ObjectPointer(element_amount, DroneDataNullObjectType, 0);

                min_object_location = data_ref.pool;
                max_object_location = data_ref.pool + ((element_amount - 1) * object_size);
            } else if (!data) {
                if (!data) {
                    std::cout << "ObjectMemPool:: Allocation amount request " << element_amount
                              << " surpasses max allocation size " << MAX_ELEMENT_SIZE << std::endl;
                    throw 0;
                }
            }
        }

        ~ObjectMemPool()
        {
            // if (data) LargeBlockAllocator::deallocate(data);
        }

        template <class T> static T * createObject()
        {
            static_assert(T::DroneDataType >= 0 && T::DroneDataType <= 255,
                          "Should Have DroneDataType Type");

            if constexpr (T::DroneDataType == 255) {
                std::cout << "ObjectMemPool:: Root Prop objects should not be constructed"
                          << std::endl;
                throw 0;
            }

            if (data->next_free < data->element_count) {

                const unsigned index = data->next_free;

                ObjectPointer data_slot = data->LU[data->next_free];

                data->next_free = data_slot.getIndex();

                T * obj = new (static_cast<void *>(data->pool + (index * object_size))) T();

                data->LU[index] = ObjectPointer(index, T::DroneDataType, data->current_uuid++);

                return obj;
            }

            return nullptr;
        };

        template <class T>
        static DroneDataHandle_<ObjectMemPool<object_size>, T> createObjectReturnRef()
        {
            return getReference(createObject<T>());
        };

        static inline unsigned indexFromPtr(const void * ptr)
        {
            return (unsigned)(((unsigned long long)ptr - (unsigned long long)min_object_location) /
                              object_size);
        }

        static inline bool isPointerInPool(const void * ptr)
        {
            bool pointer_in_min_bound = ptr >= min_object_location;
            bool pointer_in_max_bound = ptr <= max_object_location;
            bool pointer_alligned_to_size_boundary =
                (((unsigned long long)ptr % object_size) - offset_alignment) == 0;

            return pointer_in_min_bound && pointer_in_max_bound &&
                   pointer_alligned_to_size_boundary;
        }


        static ObjectPointer getReference(const void * ptr)
        {
            if (isPointerInPool(ptr)) {
                unsigned index = indexFromPtr(ptr);
                return data->LU[index];
            }

            return ObjectPointer();
        };

        template <class T>
        static void deleteObject(DroneDataHandle_<ObjectMemPool<object_size>, T> & object)
        {
            ObjectPointer obj = resolveReference(object);

            if (obj) deleteObject(obj.ptr());

            // Sneaky Sneak, change the index to MAX_ELEMENT_SIZE
            *((unsigned *)&object) = MAX_ELEMENT_SIZE;
        };

        static void deleteObject(const void * ptr)
        {
            if (isPointerInPool(ptr)) {
                unsigned index = indexFromPtr(ptr);

                data->LU[index] = ObjectPointer(data->next_free, DroneDataNullObjectType, 0);

                data->next_free = index;
            }
        };

      public:
        // Resolve object pointer
        static ObjectPointer resolveReference(ObjectPointer object)
        {

            unsigned index = object.getIndex();

            if (index < data->element_count) {

                if (data->LU[index] == object)
                    return object;
                else {
                    // Hunt down the reference, which may have been moved.
                    // TODO: There should be a guarantee that this process will not be interrupted

                    for (int i = 0; i < data->element_count; i++) {
                        if (data->LU[i] == object) {
                            // Found what we are looking for.
                            return data->LU[i]; // Clearly data must be in-place before! LU can be
                                                // dereferenced
                        }
                    }
                    // If here then the refeence should not exist. Return an empty reference.
                }
            }

            return ObjectPointer();
        }

        static void incrementIndexedReference(unsigned i){};
        static void decrementIndexedReference(unsigned i){};
        static void * retrieveIndexedPointer(unsigned index)
        {
            return static_cast<void *>(&(data->pool[index * object_size]));
        }

        static void destroyObject(ObjectPointer object) { deleteObject(object.ptr()); };

        static size_t byteSize()
        {

            if (data) return data->byte_size;
            return 0;
        }

        static bool resize() {}
    };

#define DroneDataByteSize 32

    typedef ObjectMemPool<DroneDataByteSize> DroneDataPool;
    typedef DroneDataPool::ObjectPointer DroneDataHandle;

#define ADD_DRONE_DATA_REFERENCES(prop)                                                            \
    typedef DroneDataHandle_<DroneDataPool, prop> Ref;                                             \
    typedef DroneDataHandle_<DroneDataPool, prop> RefType;                                         \
    typedef DroneDataHandle_<DroneDataPool, prop> Reference;                                       \
    typedef DroneDataHandle_<DroneDataPool, prop> ReferenceType;
} // namespace hive