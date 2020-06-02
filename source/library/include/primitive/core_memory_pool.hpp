#pragma once

#include "include/primitive/core_debug.hpp"
#include "include/primitive/core_string_hash.hpp"
#include "include/primitive/core_type_information.hpp"
#include <atomic>
#include <cstdlib>
#include <new>

namespace hive
{


    /**
     * Allocates large blocks of contiguous memory.
     */
    struct LargeBlockAllocator {
        static void * allocate(size_t size) { return malloc(size); };
        static void deallocate(void * ptr) { free(ptr); }
    };

    template <class Allocator, class Type = void *> struct DroneDataHandle_ {

        enum vTYPE : unsigned { INVALID_REF = 0 };

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

            return (type << MEM_MAX_ELEMENT_BIT_COUNT) | index;
        }

        inline unsigned getIndex() const { return index_type & MEM_INDEX_MASK; }

        inline unsigned getType() const { return index_type >> MEM_MAX_ELEMENT_BIT_COUNT; }

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

        template <class T>
        DroneDataHandle_(const DroneDataHandle_<Allocator, T> & hndl, unsigned new_index)
            : DroneDataHandle_(new_index, hndl.getType(), hndl.getID())
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

        explicit operator bool() { return (index_type) != DroneDataHandle_::INVALID_REF; }

        Type & operator*() const { return *ptr(); }

        constexpr operator Type *()
        {
            static_assert(Type::DroneDataType >= 0 &&
                              Type::DroneDataType <= MEM_MAX_ELEMENT_TYPE_COUNT - 1,
                          "Should Have DroneDataType Type");

            return ptr();
        }

        constexpr operator Type &()
        {
            static_assert(Type::DroneDataType >= 0 &&
                              Type::DroneDataType <= MEM_MAX_ELEMENT_TYPE_COUNT - 1,
                          "Should Have DroneDataType Type");

            return *ptr();
        }

        Type * operator->() { return ptr(); }

        Type * ptr() { return static_cast<Type *>(Allocator::retrieveIndexedPointer(*this)); }

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
     *
     * Uses the monostate pattern:
     *  Objects can be created using:
     *      ObjectMemPool<*>::createObject<OBJ>()
     *      or
     *      ObjectMemPool<*>().createObject<OBJ>()
     *      or
     *      ObjectMemPool<*> pool;
     *      pool.createObject<OBJ>()
     */
    template <const int object_size> struct ObjectMemPool {

        constexpr static unsigned DroneDataStructSize     = object_size + 16;
        constexpr static unsigned DroneDataNullObjectType = 0xFF;
        typedef DroneDataHandle_<ObjectMemPool<object_size>> ObjectPointer;

        static inline void * min_object_location = nullptr;
        static inline void * max_object_location = nullptr;


      private:
        struct AdvancedHeader {
            ObjectPointer ptr;
            // This will contain either a ref to the drone if the type is in the PROP group, or
            // advanced flags if the type is a DRONE,
            u_ull implementation_data;
            u_ull structure_data_A;
            u_ull structure_data_B;
        };

        static_assert(sizeof(AdvancedHeader) == DroneDataStructSize,
                      "Pool slot and DataStructure missmatch");

        typedef ObjectPointer ObjectPoolHeader;


        // TABLE FOR STORING Object Pool data
        static inline struct Data {
            unsigned element_count;
            unsigned byte_size;
            unsigned next_free;
            unsigned current_uuid;
            AdvancedHeader * Pool;
        } * data = nullptr;

      public:
        template <class T> class iterator
        {
            unsigned index         = 0;
            unsigned element_count = data->element_count;
            AdvancedHeader * Pool  = data->Pool;


          public:
            iterator(int index) : index(index) { (*this)++; }
            iterator() : index(0) { (*this)++; }

            iterator & operator++()
            {
                while (index++ < element_count) {
                    if (Pool[index].ptr.template is<T>()) return *this;
                }

                index = element_count;

                return *this;
            }

            iterator operator++(int)
            {
                while (index++ < element_count) {


                    unsigned type =
                        (*reinterpret_cast<unsigned *>(Pool + index)) >> MEM_MAX_ELEMENT_BIT_COUNT;

                    if (type == T::DroneDataType) return *this;
                }

                index = element_count;

                return *this;
            }

            bool operator==(iterator other) const { return (index == other.index); }
            bool operator!=(iterator other) const { return !(index == other.index); }
            ObjectPointer operator*() { return ObjectPointer(data->Pool[index].ptr, index); }
            // iterator traits
            using difference_type   = long;
            using value_type        = ObjectPointer;
            using pointer           = const ObjectPointer *;
            using reference         = const ObjectPointer &;
            using iterator_category = std::forward_iterator_tag;
        };
        template <class T> static iterator<T> begin() { return iterator<T>(); }
        template <class T> static iterator<T> end() { return iterator<T>(data->element_count); }

        constexpr ObjectMemPool()
        {
            if (!data)
                HIVE_ERROR("ObjectMemPool:: Cannot allocate from empty pool! Use constructor with  "
                           "ObjectMemPool(PoolSize) to "
                           "initialize poll data.");
        }

        constexpr ObjectMemPool(size_t element_amount)
        {
            if (!data && element_amount < MEM_MAX_ELEMENT_SIZE) {

                const unsigned construct_size = sizeof(Data) + element_amount * DroneDataStructSize;

                data = static_cast<Data *>(LargeBlockAllocator::allocate(construct_size));

                if (!data) {
                    std::cout << "ObjectMemPool:: unable to allocate memory for amount request: "
                              << element_amount << std::endl;
                    throw 0;
                }

                Data & data_ref        = *data;
                data_ref.element_count = element_amount;
                data_ref.byte_size     = construct_size;
                data_ref.next_free     = 1;
                data_ref.current_uuid  = 0;
                data_ref.Pool = reinterpret_cast<AdvancedHeader *>(reinterpret_cast<char *>(data) +
                                                                   sizeof(Data));

                data_ref.Pool[0].ptr = ObjectPointer();

                // Initialize Pool list
                // Using the ObjectPointer with the Null Id, the index of this handle
                // will point to the next free index slot available.
                for (int i = 1; i < element_amount - 1; i++)
                    data_ref.Pool[i].ptr = ObjectPointer(i + 1, DroneDataNullObjectType, 0);

                // The last free object will always have an index greater than the available
                // objects.
                data_ref.Pool[element_amount - 1].ptr =
                    ObjectPointer(element_amount, DroneDataNullObjectType, 0);

                min_object_location = data_ref.Pool;
                max_object_location =
                    (data_ref.Pool) + ((element_amount - 1) * DroneDataStructSize);

            } else if (!data) {
                if (!data) {
                    HIVE_FATAL_ERROR("ObjectMemPool:: Allocation amount request ", element_amount,
                                     " surpasses max allocation size ", MEM_MAX_ELEMENT_SIZE)
                }
            }
        }

        ~ObjectMemPool()
        {
            // if (data) LargeBlockAllocator::deallocate(data);
        }

        template <class T> static std::tuple<T *, unsigned> createObject()
        {
            static_assert(T::DroneDataType >= 0 && T::DroneDataType < MEM_MAX_ELEMENT_TYPE_COUNT,
                          "Should Have DroneDataType Type");

            static_assert(sizeof(T) <= DroneDataStructSize, "Object type is incorrect size");

            if constexpr (T::DroneDataType == MEM_MAX_ELEMENT_TYPE_COUNT - 1)
                HIVE_ERROR("ObjectMemPool:: Root Prop objects should not be constructed")


            if (data->next_free < data->element_count) {

                const unsigned index = data->next_free;

                ObjectPointer data_slot = data->Pool[data->next_free].ptr;

                data->next_free = data_slot.getIndex();

                hive_ull offset = ((unsigned long long)(data->Pool + index) -
                                   (unsigned long long)min_object_location);

                T * obj = new (static_cast<void *>(data->Pool + index)) T();

                data->Pool[index].ptr = ObjectPointer(0, T::DroneDataType, data->current_uuid++);

                return {obj, index};
            }

            return {nullptr, 0};
        };

        template <class T>
        static DroneDataHandle_<ObjectMemPool<object_size>, T> createObjectReturnRef()
        {
            static_assert(offsetof(T, ref) == 0,
                          "Structure should have a [ref] member at the 0th offset.");

            const auto [obj, index] = createObject<T>();

            if (obj != nullptr)
                return ObjectPointer(obj->ref, index);
            else
                return ObjectPointer();
        };


        static bool setImplementationData(const ObjectPointer & ref, const hive_ull & data_numeric)
        {
            hive_ull maskA;

            unsigned indexA = resolveIndex(ref, maskA);

            if (maskA) {
                data->Pool[indexA].implementation_data = data_numeric;
                return true;
            }

            return false;
        }

        static hive_ull getImplementationData(const ObjectPointer & ref)
        {
            hive_ull maskA;

            unsigned indexA = resolveIndex(ref, maskA);

            if (maskA) {
                return data->Pool[indexA].implementation_data;
            }

            return 0;
        }

        static inline unsigned indexFromPtr(const void * ptr)
        {
            hive_ull offset = ((unsigned long long)ptr - (unsigned long long)min_object_location);

            return (unsigned)(offset / DroneDataStructSize);
        }

        static inline bool isPointerInPool(const void * ptr)
        {

            bool pointer_in_min_bound = (ptr >= min_object_location);
            bool pointer_in_max_bound = (ptr <= max_object_location);

            hive_ull offset = ((unsigned long long)ptr - (unsigned long long)min_object_location);

            bool pointer_alligned_to_size_boundary = (offset % DroneDataStructSize) == 0;

            return pointer_in_min_bound && pointer_in_max_bound &&
                   pointer_alligned_to_size_boundary;
        }


        static ObjectPointer getReference(const void * ptr)
        {
            if (isPointerInPool(ptr)) {
                unsigned index = indexFromPtr(ptr);

                auto ref = data->Pool[index].ptr;

                return ObjectPointer(ref, index);
            }

            return ObjectPointer();
        };

        template <class T>
        static void deleteObject(DroneDataHandle_<ObjectMemPool<object_size>, T> & object)
        {

            hive_ull maskA;

            unsigned indexA = resolveIndex(object, maskA);

            if (maskA) deleteObject(indexA);

            // Sneaky Sneak, change the index to MAX_ELEMENT_SIZE
            *((unsigned *)&object) = MEM_MAX_ELEMENT_SIZE;
        };

        static void deleteObject(const unsigned index)
        {
            HIVE_DEBUG_WARN("Make this thread safe!!")

            data->Pool[index].ptr = ObjectPointer(data->next_free, DroneDataNullObjectType, 0);

            data->next_free = index;
        };

      public:
        /**
         * Resolve object pointer
         */
        static ObjectPointer resolveReference(ObjectPointer object)
        {
            hive_ull maskA;

            unsigned index = resolveIndex(object, maskA);

            if (maskA) return ObjectPointer(object, index);

            return ObjectPointer();
        }

      private:
        // TODO - move to source file.
        static unsigned resolveIndex(const ObjectPointer & ptr, hive_ull & mask)
        {
            mask = 0xFFFFFFFFFFFFFFFF;

            const unsigned index     = ptr.getIndex();
            const unsigned object_id = ptr.getID();

            // Compare index of current object with the ptr.
            if (object_id != data->Pool[index].ptr.getID()) {

                // Fast path, use Generation Resolution Lookup Table.

                HIVE_DEBUG_WARN("Need to implement remapping functions for DRONE/PROP lookup.")

                // Slow path, iterate throw array to find the object.

                for (int i = 1; i < data->element_count; i++) {
                    if (data->Pool[i].ptr.getID() == object_id) {
                        // Found what we are looking for.
                        return i;
                    }
                }

                HIVE_DEBUG_WARN("Lost reference to object:", object_id, " type", ptr.getType());

                // If the object can't be resolved return [false index]
                mask = 0;
                return 0;
            }

            return index;
        }

      public:
        /**
         * Append @arg{next_ptr} to end of chain specified by @arg{ptr}.
         */
        static bool appendRefToEndOfChain(ObjectPointer ptr, ObjectPointer next_ptr)
        {
            HIVE_DEBUG_WARN("How can this be made concurrent?")

            hive_ull maskA, maskB;

            unsigned indexA = resolveIndex(ptr, maskA), indexB = resolveIndex(next_ptr, maskB);

            if (maskA & maskB) {
                // We'll proceed down the object prop chain until we reach the end of the chain.
                ObjectPointer current = data->Pool[indexA].ptr;

                while (current.getIndex() > 0) { // TODO: Need bounds check.
                    indexA  = current.getIndex();
                    current = data->Pool[indexA].ptr;
                }
                // Could be CAS to force a redo if necessary.
                data->Pool[indexA].ptr = ObjectPointer(current, indexB);

                return true;
            }

            return false;
        }

        /**
         *  Return a reference to the next object in the chain. May return a InvalidRef ptr.
         */
        static ObjectPointer getNextRef(ObjectPointer ptr)
        {
            hive_ull maskA;

            unsigned indexA = resolveIndex(ptr, maskA);

            if (indexA) {
                ObjectPointer current = data->Pool[indexA].ptr;

                unsigned next_index = current.getIndex();

                if (next_index > 0) {
                    ObjectPointer next = data->Pool[next_index].ptr;
                    return ObjectPointer(next, next_index);
                }
            }

            // Return the InvalidRef object pointer.
            return ObjectPointer();
        }

        /**
         *  Return a reference to the next object in the chain. May return a InvalidRef ptr.
         */
        template <class T> static ObjectPointer getNextRefOfType(ObjectPointer ptr)
        {
            static_assert(T::DroneDataType >= 0 && T::DroneDataType <= MEM_MAX_ELEMENT_TYPE_COUNT,
                          "Should Have DroneDataType Type");

            hive_ull maskA;

            unsigned indexA = resolveIndex(ptr, maskA);

            if (maskA) {

                ObjectPointer current = data->Pool[indexA].ptr;

                while (current) {

                    unsigned next_index = current.getIndex();

                    if (next_index > 0) {
                        ObjectPointer next = data->Pool[next_index].ptr;
                        if (next.getType() == T::DroneDataType)
                            return ObjectPointer(next, next_index);
                    } else
                        break;
                }
            }

            // Return the InvalidRef object pointer.
            return ObjectPointer();
        }

        static void incrementIndexedReference(unsigned i){};
        static void decrementIndexedReference(unsigned i){};
        static void * retrieveIndexedPointer(const ObjectPointer & ptr)
        {
            hive_ull maskA;
            unsigned indexA = resolveIndex(ptr, maskA);
            return static_cast<void *>(data->Pool + indexA);
        }

        static void destroyObject(ObjectPointer object) { deleteObject(object.ptr()); };

        static size_t byteSize()
        {

            if (data) return data->byte_size;
            return 0;
        }

        static bool resize() {}
    }; // namespace hive


    typedef ObjectMemPool<DroneDataByteSize> DroneDataPool;
    typedef DroneDataPool::ObjectPointer DroneDataHandle;


#define ADD_DRONE_DATA_REFERENCES(prop)                                                            \
    typedef DroneDataHandle_<DroneDataPool, prop> Ref;                                             \
    typedef DroneDataHandle_<DroneDataPool, prop> RefType;                                         \
    typedef DroneDataHandle_<DroneDataPool, prop> Reference;                                       \
    typedef DroneDataHandle_<DroneDataPool, prop> ReferenceType;


    // Data Memory Pools
    struct DataPool {

      private:
        unsigned word_size = 0; // Size of memory pool, in 4 byte units.
        unsigned char * pool;

        /** Field header for a field of free memory, implemented as a single linked cyclic list.
         */
        struct FreeMemoryLink {
            FreeMemoryLink * next;
            unsigned word_size;
        } * first_free_memory;

      public:
        /**
         * Provides memory observation mechanism for
         * data fields. Allocated from DroneDataPool
         */
        struct DataChain {

            static const ushort DroneDataType = MEM_DATA_CHAIN_TYPE;
            DroneDataHandle next_observer_link;
            DroneDataHandle prev_observer_link;
        };

        static_assert(sizeof(DataChain) <= DroneDataPool::DroneDataStructSize,
                      "Prop size is greater than the pool allocation unit size");

        // DATAFIELD
        // DATAFIELD
        // DATAFIELD
        // DATAFIELD
        // DATAFIELD
        //
        //
        //
        // Field of data information
        template <class D> struct DataField {

            static_assert(sizeof(D) >= 4, "DataField structures must be 4 bytes or more.");

            DroneDataHandle data_chain = 0;

            unsigned word_size                    = 0;
            std::atomic<unsigned> reference_count = 0;

            D field;

            DataField<D>() { new (&field) D(); }

            ~DataField<D>() {}

            static void destruct(DataField<D> * d) noexcept { delete d; }

            /**
             * Moves the data to a new buffer location or returns false if a move
             * is unsuccessful.
             */
            bool move(void * new_location) { return false; };

            /**
             * Removes header from data and returns pointer to the data field.
             * This effectively makes the data static within it's buffer and all
             * dynamic memory management properties are lost, including the ability
             * to automatically update drone's when data field memory are changed.
             */
            void * degrade() { return nullptr; };

            void update(void * data)
            {
                memcpy(&field, data, sizeof(data));
                update();
            }

            void update() {}

            // get affected drones.
            void addDronePointer(DroneDataHandle _drone){};

            void removeDronePointer(DroneDataHandle drone){};

            unsigned getDroneRefCount() { return 0; };

            void getDroneRefs(DroneDataHandle[], unsigned array_size){};

            struct PointerType {

              private:
                DataField * host;

                inline D * ptr() const
                {
                    return reinterpret_cast<D *>(reinterpret_cast<char *>(host) +
                                                 offsetof(DataField, field));
                }

              public:
                PointerType(DataField * ptr) : host(ptr)
                {
                    if (host) host->reference_count++;
                }

                PointerType(const PointerType & ptr) : PointerType(ptr.host) {}

                ~PointerType()
                {
                    if (host) host->reference_count--;
                };

                D & raw() { return *ptr(); }
                D * operator->() const { return ptr(); };
                D & operator[](const size_t index) { return (ptr())[index]; };

                PointerType & operator=(const D & data)
                {
                    *ptr() = data;
                    return *this;
                }
                void update() { host->update(); };

                constexpr operator bool() { return host != nullptr; }
            };

            typedef PointerType PTR;
        };


        typedef DataField<void *> GenericDataField;

        constexpr static unsigned MemLinkWordSize = (sizeof(FreeMemoryLink) >> 2);

        DataPool(const unsigned word_size) : word_size(word_size)
        {
            // Attempt to allocate memory from system for this pool.
            void * data = LargeBlockAllocator::allocate(word_size * 4);

            if (!data) HIVE_ERROR("Unable to make alloction for DataPool");

            // Setup the first header field for free memory access.

            FreeMemoryLink & link = *reinterpret_cast<FreeMemoryLink *>(data);

            link.word_size = word_size - MemLinkWordSize;

            link.next = nullptr;

            first_free_memory = &link;
        }

        ~DataPool() {}

        /**
         * Allocate a field of data.
         */
        template <class T> typename DataField<T>::PointerType allocate()
        {
            // Need to allocate enough space for a header as well as the data.
            unsigned full_allocation_request_size = sizeof(DataField<T>);

            // Search through existing free fields and find an appropriately sized section to
            // handle the data. First fit approach implemented at this point.
            DataField<T> * df = nullptr;

            if (first_free_memory) {

                FreeMemoryLink * prev = nullptr;
                FreeMemoryLink * link = first_free_memory;
                FreeMemoryLink * next = link->next;

                while (link) {

                    next = link->next;

                    int fit =
                        (link->word_size + MemLinkWordSize) - (full_allocation_request_size >> 2);

                    if (fit >= 0) {


                        unsigned word_size = link->word_size + MemLinkWordSize;

                        df = new (reinterpret_cast<void *>(link)) DataField<T>();

                        if (fit < MemLinkWordSize << 1) {
                            // Allocate entire memory space to the object.

                            df->word_size = word_size;

                            if (prev) {
                                prev->next = next;
                            } else
                                first_free_memory = next;

                            break;
                        } else {

                            unsigned data_field_word_size = word_size - fit;

                            df->word_size = data_field_word_size;

                            // Split up free memory field.
                            FreeMemoryLink & new_link = *reinterpret_cast<FreeMemoryLink *>(
                                reinterpret_cast<char *>(link) + (data_field_word_size << 2));

                            new_link.word_size = fit - MemLinkWordSize;

                            new_link.next = next;

                            if (prev) {
                                prev->next = &new_link;
                            } else
                                first_free_memory = &new_link;

                            break;
                        }
                    } else {
                        prev = link;
                        link = link->next;
                    }
                }
            }

            return df;
        }

        /**
         * Free a field of data.
         */
        template <class T> void free(DataField<T> * data_field)
        {
            // Need to check if the data field comes from this buffer.
            void * ptr = data_field;

            if (ptr >= pool && ptr <= pool) {
                // TODO: additional checks should be made to make sure the data_field is
                // correctly aligned

                // replace the data_field with a free memory structure

                // Split up free memory field.
                FreeMemoryLink & new_link = *reinterpret_cast<FreeMemoryLink *>(data_field);

                new_link.word_size = data_field->word_size - MemLinkWordSize;

                if (first_free_memory) {
                    /*TODO: Insert in order. Merge if possible.
                    FreeMemoryLink * prev = nullptr;
                    FreeMemoryLink * link = first_free_memory;
                    FreeMemoryLink * next = link->next;

                    while (link && link < &new_link) {
                        prev = link;
                        link = link->next;
                        next = link->next;
                    }
                    */
                    new_link.next     = first_free_memory;
                    first_free_memory = &new_link;
                } else {
                    new_link.next = nullptr;
                }
            } else {
                HIVE_ERROR("DataPool:: Attempt to free data_field that does not belong to the "
                           "DataPool.")
            }
        }

        /**
         * Transfer data element from this pool to another pool.
         *
         * Alert any watchers of the the new location for the
         * data.
         */
        void transfer(GenericDataField & field, DataPool & pool);
    };

    // Create a general pool that can store default data. This should be a global object.
    // Bosses MAY have additional pools that can be used to optimize their memory cache
    // performance. This pool should be defined at program entry.
    extern DataPool general_data_pool;

} // namespace hive