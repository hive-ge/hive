/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    DO NOT MODIFY - This file is automatically created by the build:props script.

    COPYRIGHT 2020 Anthony C Weathersby
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
 #pragma once
#include <thread>
#include <vector>

#include <nodejs/src/node_api.h>
#include <hive.h>

using namespace hive::all;

namespace hive{
    namespace js{

        template<class T>
        T * getPropPointer(napi_env env, napi_value value){
            void * ptr;

            napi_status status = napi_unwrap(env, value, &ptr);

            if(status != napi_ok)
                return nullptr;
            
            return static_cast<T *>(ptr);
        }

        napi_value JSError(napi_env env, const char * message){
            napi_throw_error(env, nullptr, message);
            return nullptr;
        }

        napi_ref Prop_reference;
napi_ref Drone_reference; 

        //Forward Declarations

        napi_value Prop_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR);
napi_value Drone_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR);

        
napi_value Prop_drone_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                Drone * v = prop -> drone;

            if (v == nullptr) {
                status = napi_get_null(env, & value);
            } else {

                //Wrap the pointer within an accessor object. 
                value = Drone_wrapped_instance(v, env, info, false);
            }

                return value;
}

napi_value Prop_drone_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                /** 
             * Need to test if the incoming value is an object, is a wrapped object, and is a property with a type that
             * matches the struct member type.
             */
            bool IS_INSTANCE = false;
            napi_valuetype value_type;

            status = napi_typeof(env, args[0], &value_type);

            if (status != napi_ok) {
                //TODO - Throw JS error
            } else if (value_type == napi_null) {
                prop -> drone = nullptr;
            } else {
                napi_value construct;

                status = napi_get_reference_value(env, Drone_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference Drone_reference");

                status = napi_instanceof(env, args[0], construct, & IS_INSTANCE);

                if (status == napi_ok && IS_INSTANCE) {

                    //Unbox the JS value;
                    Drone * val = getPropPointer < Drone> (env, args[0]);

                    prop -> drone = val;

                    std:: cout << "Assigning pointer val drone" << (long long) val << std:: endl;
                } else
                    std:: cout << "Unable to to assign pointer Drone" << (long long) args[0] << std:: endl;
            } 

                return value;
}

napi_value Prop_prev_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                Prop * v = prop -> prev;

            if (v == nullptr) {
                status = napi_get_null(env, & value);
            } else {

                //Wrap the pointer within an accessor object. 
                value = Prop_wrapped_instance(v, env, info, false);
            }

                return value;
}

napi_value Prop_prev_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                /** 
             * Need to test if the incoming value is an object, is a wrapped object, and is a property with a type that
             * matches the struct member type.
             */
            bool IS_INSTANCE = false;
            napi_valuetype value_type;

            status = napi_typeof(env, args[0], &value_type);

            if (status != napi_ok) {
                //TODO - Throw JS error
            } else if (value_type == napi_null) {
                prop -> prev = nullptr;
            } else {
                napi_value construct;

                status = napi_get_reference_value(env, Prop_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference Prop_reference");

                status = napi_instanceof(env, args[0], construct, & IS_INSTANCE);

                if (status == napi_ok && IS_INSTANCE) {

                    //Unbox the JS value;
                    Prop * val = getPropPointer < Prop> (env, args[0]);

                    prop -> prev = val;

                    std:: cout << "Assigning pointer val prev" << (long long) val << std:: endl;
                } else
                    std:: cout << "Unable to to assign pointer Prop" << (long long) args[0] << std:: endl;
            } 

                return value;
}

napi_value Prop_next_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                Prop * v = prop -> next;

            if (v == nullptr) {
                status = napi_get_null(env, & value);
            } else {

                //Wrap the pointer within an accessor object. 
                value = Prop_wrapped_instance(v, env, info, false);
            }

                return value;
}

napi_value Prop_next_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                /** 
             * Need to test if the incoming value is an object, is a wrapped object, and is a property with a type that
             * matches the struct member type.
             */
            bool IS_INSTANCE = false;
            napi_valuetype value_type;

            status = napi_typeof(env, args[0], &value_type);

            if (status != napi_ok) {
                //TODO - Throw JS error
            } else if (value_type == napi_null) {
                prop -> next = nullptr;
            } else {
                napi_value construct;

                status = napi_get_reference_value(env, Prop_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference Prop_reference");

                status = napi_instanceof(env, args[0], construct, & IS_INSTANCE);

                if (status == napi_ok && IS_INSTANCE) {

                    //Unbox the JS value;
                    Prop * val = getPropPointer < Prop> (env, args[0]);

                    prop -> next = val;

                    std:: cout << "Assigning pointer val next" << (long long) val << std:: endl;
                } else
                    std:: cout << "Unable to to assign pointer Prop" << (long long) args[0] << std:: endl;
            } 

                return value;
}

napi_value Prop_type_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                status = napi_create_uint32(env, static_cast<unsigned>(prop -> type), &value);
                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned short integer");

                return value;
}

napi_value Prop_type_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                unsigned temp_unsigned;
                status = napi_get_value_uint32(env, args[0], (unsigned *) &temp_unsigned);
                prop->type = static_cast<unsigned short>(prop -> type);
                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned short integer");

                return value;
}

napi_value Prop_byte_size_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                status = napi_create_uint32(env, static_cast<unsigned>(prop -> byte_size), &value);
                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned short integer");

                return value;
}

napi_value Prop_byte_size_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Prop * prop = (Prop*)data;

                unsigned temp_unsigned;
                status = napi_get_value_uint32(env, args[0], (unsigned *) &temp_unsigned);
                prop->byte_size = static_cast<unsigned short>(prop -> byte_size);
                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned short integer");

                return value;
}

        napi_value Prop_connect_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            bool IS_INSTANCE = false;
            napi_value construct;
                status = napi_get_reference_value(env, Drone_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference Drone_reference");

                status = napi_instanceof(env, args[0], construct, & IS_INSTANCE);

                if (status != napi_ok || !IS_INSTANCE) return JSError(env, "Expected instance of Drone for argument A.");

                Drone * A = getPropPointer < Drone> (env, args[0]); 
            

            Prop * prop = (Prop*)data;

            prop->connect(A);

            return return_val;
        }

        napi_value Prop_disconnect_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(0);

        size_t arg_count = 0;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            

            Prop * prop = (Prop*)data;

            prop->disconnect();

            return return_val;
        }
const napi_property_descriptor Prop_props[] = {{
                "drone",
                NULL,
                NULL,
                Prop_drone_getter,
                Prop_drone_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            },{
                "prev",
                NULL,
                NULL,
                Prop_prev_getter,
                Prop_prev_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            },{
                "next",
                NULL,
                NULL,
                Prop_next_getter,
                Prop_next_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            },{
                "type",
                NULL,
                NULL,
                Prop_type_getter,
                Prop_type_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            },{
                "byte_size",
                NULL,
                NULL,
                Prop_byte_size_getter,
                Prop_byte_size_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            },{
            "connect",
            NULL,
            Prop_connect_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "disconnect",
            NULL,
            Prop_disconnect_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        }};

    void Prop_Destructor(napi_env env, void * finalize_data, void * finalize_hint){
        
        Prop * prop = (Prop *)finalize_data;
        
        std::cout << "Deleting instance of Prop " << (long long)prop << std::endl;
        
        delete prop;
    }
napi_value Prop_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR){
        
        napi_status status;
        napi_value constructor, object, pointer_arg;

        status = napi_get_reference_value(env, Prop_reference, &constructor);
        
        if(status != napi_ok)
            std::cout << "Unable to dereference Prop_reference" << std::endl;

        status = napi_create_external( env, hive_prop, nullptr, nullptr, &pointer_arg);

        if(status != napi_ok)
            std::cout << "Unable to create a raw pointer reference" << std::endl;
        
        status = napi_new_instance(env, constructor, 1, &pointer_arg, &object);

        if(status != napi_ok)
            std::cout << "Unable to create JS instance of Prop" << std::endl; 
        
        return object;
    }

    napi_value Prop_Constructor(napi_env env, napi_callback_info info){
        
        Prop * hive_prop;

        // Create JS object.
        napi_status status;
        size_t arg_count = 1;
        napi_value this_arg, raw_arg;
        napi_valuetype arg_type;
        bool WRAP_EXISTING_POINTER = false;

        status = napi_get_cb_info(env, info, &arg_count, &raw_arg, &this_arg, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to get callback info for Prop_Constructor" << std::endl;
        
        if(arg_count == 1){

            status = napi_typeof(env, raw_arg, &arg_type);

            if(status != napi_ok)
                std::cout << "Unable to get callback info for Prop_Constructor" << std::endl;
            else if(arg_type == napi_external){
                void * ptr;

                status = napi_get_value_external(env, raw_arg, &ptr);

                if(status == napi_ok){   

                    WRAP_EXISTING_POINTER = true;

                    hive_prop = static_cast<Prop *>(ptr);
                    
                    std::cout << "wrapping instance of Prop " << (long long)hive_prop << std::endl;
                }
            }
        }
        
        if(!WRAP_EXISTING_POINTER){
            hive_prop = Prop::construct();

            std::cout << "creating instance of Prop " << (long long)hive_prop << std::endl;
        }

        if(WRAP_EXISTING_POINTER)
            status = napi_wrap(env, this_arg, hive_prop, nullptr, nullptr, nullptr);
        else
            status = napi_wrap(env, this_arg, hive_prop, Prop_Destructor, nullptr, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to wrap info for Prop_Constructor" << std::endl;

        return this_arg;
    }

        void Prop_Register(napi_env env, napi_value property_registry){
            
            napi_status status;
            
            napi_value result;

            status = napi_define_class(env, "Prop", NAPI_AUTO_LENGTH, &Prop_Constructor,
                nullptr, 7, Prop_props, &result);

            status = napi_set_named_property(env, property_registry, "Prop", result);

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE  CLASS CONSTRUCTOR FOR Prop" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR Prop" << std::endl;
            }

            status = napi_create_reference(env, result, 1, &Prop_reference);

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE REFERENCE FOR Prop" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR Prop" << std::endl;
            }
        }

napi_value Drone_id_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Drone * prop = (Drone*)data;

                /* VOID hive_struct_name: ID napi_value_name: value hive_prop_instance_name: prop hive_prop_instance_member: id*/

                return value;
}

napi_value Drone_id_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Drone * prop = (Drone*)data;

                /* VOID hive_struct_name: ID napi_value_name: args[0] hive_prop_instance_name: prop hive_prop_instance_member: id*/

                return value;
}

napi_value Drone_flags_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Drone * prop = (Drone*)data;

                status = napi_create_int64(env, static_cast<long long>(prop -> flags), &value);
                if(status != napi_ok) return JSError(env, "Could not read value as a long integer");

                return value;
}

napi_value Drone_flags_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Drone * prop = (Drone*)data;

                int64_t temp;
            status = napi_get_value_int64(env, args[0], &temp);
            if(status != napi_ok) return JSError(env, "Could not read value as a long integer");
            prop -> flags = static_cast<unsigned long long>(temp);

                return value;
}

napi_value Drone_props_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Drone * prop = (Drone*)data;

                Prop * v = prop -> props;

            if (v == nullptr) {
                status = napi_get_null(env, & value);
            } else {

                //Wrap the pointer within an accessor object. 
                value = Prop_wrapped_instance(v, env, info, false);
            }

                return value;
}

napi_value Drone_props_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                Drone * prop = (Drone*)data;

                /** 
             * Need to test if the incoming value is an object, is a wrapped object, and is a property with a type that
             * matches the struct member type.
             */
            bool IS_INSTANCE = false;
            napi_valuetype value_type;

            status = napi_typeof(env, args[0], &value_type);

            if (status != napi_ok) {
                //TODO - Throw JS error
            } else if (value_type == napi_null) {
                prop -> props = nullptr;
            } else {
                napi_value construct;

                status = napi_get_reference_value(env, Prop_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference Prop_reference");

                status = napi_instanceof(env, args[0], construct, & IS_INSTANCE);

                if (status == napi_ok && IS_INSTANCE) {

                    //Unbox the JS value;
                    Prop * val = getPropPointer < Prop> (env, args[0]);

                    prop -> props = val;

                    std:: cout << "Assigning pointer val props" << (long long) val << std:: endl;
                } else
                    std:: cout << "Unable to to assign pointer Prop" << (long long) args[0] << std:: endl;
            } 

                return value;
}

        napi_value Drone_getBoss_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(0);

        size_t arg_count = 0;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            

            Drone * prop = (Drone*)data;

            prop->getBoss();

            return return_val;
        }

        napi_value Drone_signalUpdate_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(0);

        size_t arg_count = 0;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            

            Drone * prop = (Drone*)data;

            prop->signalUpdate();

            return return_val;
        }

        napi_value Drone_connect_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            bool IS_INSTANCE = false;
            napi_value construct;
                status = napi_get_reference_value(env, Prop_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference Prop_reference");

                status = napi_instanceof(env, args[0], construct, & IS_INSTANCE);

                if (status != napi_ok || !IS_INSTANCE) return JSError(env, "Expected instance of Prop for argument A.");

                Prop * A = getPropPointer < Prop> (env, args[0]); 
            

            Drone * prop = (Drone*)data;

            prop->connect(A);

            return return_val;
        }

        napi_value Drone_disconnect_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            bool IS_INSTANCE = false;
            napi_value construct;
                status = napi_get_reference_value(env, Prop_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference Prop_reference");

                status = napi_instanceof(env, args[0], construct, & IS_INSTANCE);

                if (status != napi_ok || !IS_INSTANCE) return JSError(env, "Expected instance of Prop for argument A.");

                Prop * A = getPropPointer < Prop> (env, args[0]); 
            

            Drone * prop = (Drone*)data;

            prop->disconnect(A);

            return return_val;
        }
const napi_property_descriptor Drone_props[] = {{
                "id",
                NULL,
                NULL,
                Drone_id_getter,
                Drone_id_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            },{
                "flags",
                NULL,
                NULL,
                Drone_flags_getter,
                Drone_flags_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            },{
                "props",
                NULL,
                NULL,
                Drone_props_getter,
                Drone_props_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            },{
            "getBoss",
            NULL,
            Drone_getBoss_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "signalUpdate",
            NULL,
            Drone_signalUpdate_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "connect",
            NULL,
            Drone_connect_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "disconnect",
            NULL,
            Drone_disconnect_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        }};

    void Drone_Destructor(napi_env env, void * finalize_data, void * finalize_hint){
        
        Drone * prop = (Drone *)finalize_data;
        
        std::cout << "Deleting instance of Drone " << (long long)prop << std::endl;
        
        delete prop;
    }
napi_value Drone_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR){
        
        napi_status status;
        napi_value constructor, object, pointer_arg;

        status = napi_get_reference_value(env, Drone_reference, &constructor);
        
        if(status != napi_ok)
            std::cout << "Unable to dereference Drone_reference" << std::endl;

        status = napi_create_external( env, hive_prop, nullptr, nullptr, &pointer_arg);

        if(status != napi_ok)
            std::cout << "Unable to create a raw pointer reference" << std::endl;
        
        status = napi_new_instance(env, constructor, 1, &pointer_arg, &object);

        if(status != napi_ok)
            std::cout << "Unable to create JS instance of Drone" << std::endl; 
        
        return object;
    }

    napi_value Drone_Constructor(napi_env env, napi_callback_info info){
        
        Drone * hive_prop;

        // Create JS object.
        napi_status status;
        size_t arg_count = 1;
        napi_value this_arg, raw_arg;
        napi_valuetype arg_type;
        bool WRAP_EXISTING_POINTER = false;

        status = napi_get_cb_info(env, info, &arg_count, &raw_arg, &this_arg, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to get callback info for Drone_Constructor" << std::endl;
        
        if(arg_count == 1){

            status = napi_typeof(env, raw_arg, &arg_type);

            if(status != napi_ok)
                std::cout << "Unable to get callback info for Drone_Constructor" << std::endl;
            else if(arg_type == napi_external){
                void * ptr;

                status = napi_get_value_external(env, raw_arg, &ptr);

                if(status == napi_ok){   

                    WRAP_EXISTING_POINTER = true;

                    hive_prop = static_cast<Drone *>(ptr);
                    
                    std::cout << "wrapping instance of Drone " << (long long)hive_prop << std::endl;
                }
            }
        }
        
        if(!WRAP_EXISTING_POINTER){
            hive_prop = Drone::construct();

            std::cout << "creating instance of Drone " << (long long)hive_prop << std::endl;
        }

        if(WRAP_EXISTING_POINTER)
            status = napi_wrap(env, this_arg, hive_prop, nullptr, nullptr, nullptr);
        else
            status = napi_wrap(env, this_arg, hive_prop, Drone_Destructor, nullptr, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to wrap info for Drone_Constructor" << std::endl;

        return this_arg;
    }

        void Drone_Register(napi_env env, napi_value property_registry){
            
            napi_status status;
            
            napi_value result;

            status = napi_define_class(env, "Drone", NAPI_AUTO_LENGTH, &Drone_Constructor,
                nullptr, 7, Drone_props, &result);

            status = napi_set_named_property(env, property_registry, "Drone", result);

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE  CLASS CONSTRUCTOR FOR Drone" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR Drone" << std::endl;
            }

            status = napi_create_reference(env, result, 1, &Drone_reference);

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE REFERENCE FOR Drone" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR Drone" << std::endl;
            }
        }
        
        void RegisterInterfaces(napi_env env, napi_value registry){
            Prop_Register(env, registry);
Drone_Register(env, registry);

        }
    }
}