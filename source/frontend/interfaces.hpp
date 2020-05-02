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

        void napi_inherits(napi_env env, napi_value ctor, napi_value super_ctor) {
            napi_value global, global_object, set_proto, ctor_proto_prop, super_ctor_proto_prop;
            napi_value args[2];
        
            napi_get_global(env, &global);
            napi_get_named_property(env, global, "Object", &global_object);
            napi_get_named_property(env, global_object, "setPrototypeOf", &set_proto);
            napi_get_named_property(env, ctor, "prototype", &ctor_proto_prop);
            napi_get_named_property(env, super_ctor, "prototype", &super_ctor_proto_prop);
        
            args[0] = ctor_proto_prop;
            args[1] = super_ctor_proto_prop;
            napi_call_function(env, global, set_proto, 2, args, NULL);
        
            args[0] = ctor;
            args[1] = super_ctor;
            napi_call_function(env, global, set_proto, 2, args, NULL);
        }

        napi_ref Prop_reference;
napi_ref SpriteProp_reference;
napi_ref Drone_reference;
napi_ref MeshProp_reference;
napi_ref ShaderProgramProp_reference; 

        //Forward Declarations

        napi_value Prop_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR);
napi_value SpriteProp_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR);
napi_value Drone_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR);
napi_value MeshProp_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR);
napi_value ShaderProgramProp_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR);

        
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
                nullptr, 6, Prop_props, &result);

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

napi_value SpriteProp_sprite_id_getter(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                SpriteProp * prop = (SpriteProp*)data;

                status = napi_create_uint32(env, prop -> sprite_id, & value);

                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned integer");

                return value;
}

napi_value SpriteProp_sprite_id_setter(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

                SpriteProp * prop = (SpriteProp*)data;

                status = napi_get_value_uint32(env, args[0], (unsigned *) & (prop -> sprite_id));

                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned integer");

                return value;
}
const napi_property_descriptor SpriteProp_props[] = {{
                "sprite_id",
                NULL,
                NULL,
                SpriteProp_sprite_id_getter,
                SpriteProp_sprite_id_setter,
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            }};

    void SpriteProp_Destructor(napi_env env, void * finalize_data, void * finalize_hint){
        
        SpriteProp * prop = (SpriteProp *)finalize_data;
        
        std::cout << "Deleting instance of SpriteProp " << (long long)prop << std::endl;
        
        delete prop;
    }
napi_value SpriteProp_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR){
        
        napi_status status;
        napi_value constructor, object, pointer_arg;

        status = napi_get_reference_value(env, SpriteProp_reference, &constructor);
        
        if(status != napi_ok)
            std::cout << "Unable to dereference SpriteProp_reference" << std::endl;

        status = napi_create_external( env, hive_prop, nullptr, nullptr, &pointer_arg);

        if(status != napi_ok)
            std::cout << "Unable to create a raw pointer reference" << std::endl;
        
        status = napi_new_instance(env, constructor, 1, &pointer_arg, &object);

        if(status != napi_ok)
            std::cout << "Unable to create JS instance of SpriteProp" << std::endl; 
        
        return object;
    }

    napi_value SpriteProp_Constructor(napi_env env, napi_callback_info info){
        
        SpriteProp * hive_prop;

        // Create JS object.
        napi_status status;
        size_t arg_count = 1;
        napi_value this_arg, raw_arg;
        napi_valuetype arg_type;
        bool WRAP_EXISTING_POINTER = false;

        status = napi_get_cb_info(env, info, &arg_count, &raw_arg, &this_arg, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to get callback info for SpriteProp_Constructor" << std::endl;
        
        if(arg_count == 1){

            status = napi_typeof(env, raw_arg, &arg_type);

            if(status != napi_ok)
                std::cout << "Unable to get callback info for SpriteProp_Constructor" << std::endl;
            else if(arg_type == napi_external){
                void * ptr;

                status = napi_get_value_external(env, raw_arg, &ptr);

                if(status == napi_ok){   

                    WRAP_EXISTING_POINTER = true;

                    hive_prop = static_cast<SpriteProp *>(ptr);
                    
                    std::cout << "wrapping instance of SpriteProp " << (long long)hive_prop << std::endl;
                }
            }
        }
        
        if(!WRAP_EXISTING_POINTER){
            hive_prop = SpriteProp::construct();

            std::cout << "creating instance of SpriteProp " << (long long)hive_prop << std::endl;
        }

        if(WRAP_EXISTING_POINTER)
            status = napi_wrap(env, this_arg, hive_prop, nullptr, nullptr, nullptr);
        else
            status = napi_wrap(env, this_arg, hive_prop, SpriteProp_Destructor, nullptr, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to wrap info for SpriteProp_Constructor" << std::endl;


        return this_arg;
    }

        void SpriteProp_Register(napi_env env, napi_value property_registry){
            
            napi_status status;
            
            napi_value result;

            status = napi_define_class(env, "SpriteProp", NAPI_AUTO_LENGTH, &SpriteProp_Constructor,
                nullptr, 1, SpriteProp_props, &result);

            status = napi_set_named_property(env, property_registry, "SpriteProp", result);

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE  CLASS CONSTRUCTOR FOR SpriteProp" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR SpriteProp" << std::endl;
            }

            status = napi_create_reference(env, result, 1, &SpriteProp_reference);


        
    napi_value super_constr, constr;

    status = napi_get_reference_value(env, Prop_reference, &super_constr);

    if (status != napi_ok) {JSError(env, "Unable to dereference Prop_reference"); return; };

    status = napi_get_reference_value(env, SpriteProp_reference, &constr);

    if (status != napi_ok)  {JSError(env, "Unable to dereference SpriteProp_reference"); return; };

    napi_inherits(env, constr, super_constr);
    
    

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE REFERENCE FOR SpriteProp" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR SpriteProp" << std::endl;
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
                nullptr, 4, Drone_props, &result);

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

        napi_value MeshProp_addVertex_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(5);

        size_t arg_count = 5;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            napi_valuetype number_valA;
                
                status = napi_typeof(env, args[0], &number_valA);
                
                if(status != napi_ok) return JSError(env, "Could not read argument A.");
                if(number_valA != napi_number) return JSError(env, "Argument A is not a number type.");

                double temp_A;
                float A;
    
                status = napi_get_value_double(env, args[0], &temp_A);

                if (status != napi_ok) return JSError(env, "Could not read argument A.");
                
                A = static_cast<float>(temp_A);
napi_valuetype number_valB;
                
                status = napi_typeof(env, args[1], &number_valB);
                
                if(status != napi_ok) return JSError(env, "Could not read argument B.");
                if(number_valB != napi_number) return JSError(env, "Argument B is not a number type.");

                double temp_B;
                float B;
    
                status = napi_get_value_double(env, args[1], &temp_B);

                if (status != napi_ok) return JSError(env, "Could not read argument B.");
                
                B = static_cast<float>(temp_B);
napi_valuetype number_valC;
                
                status = napi_typeof(env, args[2], &number_valC);
                
                if(status != napi_ok) return JSError(env, "Could not read argument C.");
                if(number_valC != napi_number) return JSError(env, "Argument C is not a number type.");

                double temp_C;
                float C;
    
                status = napi_get_value_double(env, args[2], &temp_C);

                if (status != napi_ok) return JSError(env, "Could not read argument C.");
                
                C = static_cast<float>(temp_C);
napi_valuetype number_valD;
                
                status = napi_typeof(env, args[3], &number_valD);
                
                if(status != napi_ok) return JSError(env, "Could not read argument D.");
                if(number_valD != napi_number) return JSError(env, "Argument D is not a number type.");

                double temp_D;
                float D;
    
                status = napi_get_value_double(env, args[3], &temp_D);

                if (status != napi_ok) return JSError(env, "Could not read argument D.");
                
                D = static_cast<float>(temp_D);
napi_valuetype number_valE;
                
                status = napi_typeof(env, args[4], &number_valE);
                
                if(status != napi_ok) return JSError(env, "Could not read argument E.");
                if(number_valE != napi_number) return JSError(env, "Argument E is not a number type.");

                double temp_E;
                float E;
    
                status = napi_get_value_double(env, args[4], &temp_E);

                if (status != napi_ok) return JSError(env, "Could not read argument E.");
                
                E = static_cast<float>(temp_E);

            MeshProp * prop = (MeshProp*)data;

            prop->addVertex(A,B,C,D,E);

            return return_val;
        }
const napi_property_descriptor MeshProp_props[] = {{
            "addVertex",
            NULL,
            MeshProp_addVertex_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        }};

    void MeshProp_Destructor(napi_env env, void * finalize_data, void * finalize_hint){
        
        MeshProp * prop = (MeshProp *)finalize_data;
        
        std::cout << "Deleting instance of MeshProp " << (long long)prop << std::endl;
        
        delete prop;
    }
napi_value MeshProp_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR){
        
        napi_status status;
        napi_value constructor, object, pointer_arg;

        status = napi_get_reference_value(env, MeshProp_reference, &constructor);
        
        if(status != napi_ok)
            std::cout << "Unable to dereference MeshProp_reference" << std::endl;

        status = napi_create_external( env, hive_prop, nullptr, nullptr, &pointer_arg);

        if(status != napi_ok)
            std::cout << "Unable to create a raw pointer reference" << std::endl;
        
        status = napi_new_instance(env, constructor, 1, &pointer_arg, &object);

        if(status != napi_ok)
            std::cout << "Unable to create JS instance of MeshProp" << std::endl; 
        
        return object;
    }

    napi_value MeshProp_Constructor(napi_env env, napi_callback_info info){
        
        MeshProp * hive_prop;

        // Create JS object.
        napi_status status;
        size_t arg_count = 1;
        napi_value this_arg, raw_arg;
        napi_valuetype arg_type;
        bool WRAP_EXISTING_POINTER = false;

        status = napi_get_cb_info(env, info, &arg_count, &raw_arg, &this_arg, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to get callback info for MeshProp_Constructor" << std::endl;
        
        if(arg_count == 1){

            status = napi_typeof(env, raw_arg, &arg_type);

            if(status != napi_ok)
                std::cout << "Unable to get callback info for MeshProp_Constructor" << std::endl;
            else if(arg_type == napi_external){
                void * ptr;

                status = napi_get_value_external(env, raw_arg, &ptr);

                if(status == napi_ok){   

                    WRAP_EXISTING_POINTER = true;

                    hive_prop = static_cast<MeshProp *>(ptr);
                    
                    std::cout << "wrapping instance of MeshProp " << (long long)hive_prop << std::endl;
                }
            }
        }
        
        if(!WRAP_EXISTING_POINTER){
            hive_prop = MeshProp::construct();

            std::cout << "creating instance of MeshProp " << (long long)hive_prop << std::endl;
        }

        if(WRAP_EXISTING_POINTER)
            status = napi_wrap(env, this_arg, hive_prop, nullptr, nullptr, nullptr);
        else
            status = napi_wrap(env, this_arg, hive_prop, MeshProp_Destructor, nullptr, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to wrap info for MeshProp_Constructor" << std::endl;


        return this_arg;
    }

        void MeshProp_Register(napi_env env, napi_value property_registry){
            
            napi_status status;
            
            napi_value result;

            status = napi_define_class(env, "MeshProp", NAPI_AUTO_LENGTH, &MeshProp_Constructor,
                nullptr, 1, MeshProp_props, &result);

            status = napi_set_named_property(env, property_registry, "MeshProp", result);

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE  CLASS CONSTRUCTOR FOR MeshProp" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR MeshProp" << std::endl;
            }

            status = napi_create_reference(env, result, 1, &MeshProp_reference);


        

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE REFERENCE FOR MeshProp" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR MeshProp" << std::endl;
            }
        }

        napi_value ShaderProgramProp_configureProgramInterfaces_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(0);

        size_t arg_count = 0;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            prop->configureProgramInterfaces();

            return return_val;
        }

        napi_value ShaderProgramProp_setupStaticInputs_method(napi_env env, napi_callback_info info){

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
            

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            prop->setupStaticInputs(A);

            return return_val;
        }

        napi_value ShaderProgramProp_addVertexShader_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            napi_valuetype number_valA;
                
                status = napi_typeof(env, args[0], &number_valA);
                
                if(status != napi_ok) return JSError(env, "Could not read argument A.");
                if(number_valA != napi_string) return JSError(env, "Argument A is not a number type.");

                size_t string_length_A = 0;
                
                status = napi_get_value_string_utf8(env, args[0], nullptr, 0, &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::vector<char> temp_A(string_length_A + 1);

                status = napi_get_value_string_utf8(env, args[0], temp_A.data(), temp_A.size(), &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::string A(temp_A.begin(), temp_A.end());

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            unsigned val = prop->addVertexShader(A);
            status = napi_create_uint32(env, val, &return_val);
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");

            return return_val;
        }

        napi_value ShaderProgramProp_addFragShader_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            napi_valuetype number_valA;
                
                status = napi_typeof(env, args[0], &number_valA);
                
                if(status != napi_ok) return JSError(env, "Could not read argument A.");
                if(number_valA != napi_string) return JSError(env, "Argument A is not a number type.");

                size_t string_length_A = 0;
                
                status = napi_get_value_string_utf8(env, args[0], nullptr, 0, &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::vector<char> temp_A(string_length_A + 1);

                status = napi_get_value_string_utf8(env, args[0], temp_A.data(), temp_A.size(), &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::string A(temp_A.begin(), temp_A.end());

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            unsigned val = prop->addFragShader(A);
            status = napi_create_uint32(env, val, &return_val);
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");

            return return_val;
        }

        napi_value ShaderProgramProp_addComputeShader_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            napi_valuetype number_valA;
                
                status = napi_typeof(env, args[0], &number_valA);
                
                if(status != napi_ok) return JSError(env, "Could not read argument A.");
                if(number_valA != napi_string) return JSError(env, "Argument A is not a number type.");

                size_t string_length_A = 0;
                
                status = napi_get_value_string_utf8(env, args[0], nullptr, 0, &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::vector<char> temp_A(string_length_A + 1);

                status = napi_get_value_string_utf8(env, args[0], temp_A.data(), temp_A.size(), &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::string A(temp_A.begin(), temp_A.end());

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            unsigned val = prop->addComputeShader(A);
            status = napi_create_uint32(env, val, &return_val);
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");

            return return_val;
        }

        napi_value ShaderProgramProp_addGeometryShader_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            napi_valuetype number_valA;
                
                status = napi_typeof(env, args[0], &number_valA);
                
                if(status != napi_ok) return JSError(env, "Could not read argument A.");
                if(number_valA != napi_string) return JSError(env, "Argument A is not a number type.");

                size_t string_length_A = 0;
                
                status = napi_get_value_string_utf8(env, args[0], nullptr, 0, &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::vector<char> temp_A(string_length_A + 1);

                status = napi_get_value_string_utf8(env, args[0], temp_A.data(), temp_A.size(), &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::string A(temp_A.begin(), temp_A.end());

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            unsigned val = prop->addGeometryShader(A);
            status = napi_create_uint32(env, val, &return_val);
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");

            return return_val;
        }

        napi_value ShaderProgramProp_addTesselationEvalShader_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            napi_valuetype number_valA;
                
                status = napi_typeof(env, args[0], &number_valA);
                
                if(status != napi_ok) return JSError(env, "Could not read argument A.");
                if(number_valA != napi_string) return JSError(env, "Argument A is not a number type.");

                size_t string_length_A = 0;
                
                status = napi_get_value_string_utf8(env, args[0], nullptr, 0, &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::vector<char> temp_A(string_length_A + 1);

                status = napi_get_value_string_utf8(env, args[0], temp_A.data(), temp_A.size(), &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::string A(temp_A.begin(), temp_A.end());

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            unsigned val = prop->addTesselationEvalShader(A);
            status = napi_create_uint32(env, val, &return_val);
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");

            return return_val;
        }

        napi_value ShaderProgramProp_addTesselationControlShader_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(1);

        size_t arg_count = 1;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            napi_valuetype number_valA;
                
                status = napi_typeof(env, args[0], &number_valA);
                
                if(status != napi_ok) return JSError(env, "Could not read argument A.");
                if(number_valA != napi_string) return JSError(env, "Argument A is not a number type.");

                size_t string_length_A = 0;
                
                status = napi_get_value_string_utf8(env, args[0], nullptr, 0, &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::vector<char> temp_A(string_length_A + 1);

                status = napi_get_value_string_utf8(env, args[0], temp_A.data(), temp_A.size(), &string_length_A);
                if (status != napi_ok) return JSError(env, "Could not read argument A.");

                std::string A(temp_A.begin(), temp_A.end());

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            unsigned val = prop->addTesselationControlShader(A);
            status = napi_create_uint32(env, val, &return_val);
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");

            return return_val;
        }

        napi_value ShaderProgramProp_compileShader_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(0);

        size_t arg_count = 0;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            unsigned val = prop->compileShader();
            status = napi_create_uint32(env, val, &return_val);
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");

            return return_val;
        }

        napi_value ShaderProgramProp_configureProgramInputs_method(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
              void * data;

        napi_value this_arg;

        std::vector<napi_value> args(0);

        size_t arg_count = 0;

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    

            

            ShaderProgramProp * prop = (ShaderProgramProp*)data;

            prop->configureProgramInputs();

            return return_val;
        }
const napi_property_descriptor ShaderProgramProp_props[] = {{
            "configureProgramInterfaces",
            NULL,
            ShaderProgramProp_configureProgramInterfaces_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "setupStaticInputs",
            NULL,
            ShaderProgramProp_setupStaticInputs_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "addVertexShader",
            NULL,
            ShaderProgramProp_addVertexShader_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "addFragShader",
            NULL,
            ShaderProgramProp_addFragShader_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "addComputeShader",
            NULL,
            ShaderProgramProp_addComputeShader_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "addGeometryShader",
            NULL,
            ShaderProgramProp_addGeometryShader_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "addTesselationEvalShader",
            NULL,
            ShaderProgramProp_addTesselationEvalShader_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "addTesselationControlShader",
            NULL,
            ShaderProgramProp_addTesselationControlShader_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "compileShader",
            NULL,
            ShaderProgramProp_compileShader_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        },{
            "configureProgramInputs",
            NULL,
            ShaderProgramProp_configureProgramInputs_method,
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        }};

    void ShaderProgramProp_Destructor(napi_env env, void * finalize_data, void * finalize_hint){
        
        ShaderProgramProp * prop = (ShaderProgramProp *)finalize_data;
        
        std::cout << "Deleting instance of ShaderProgramProp " << (long long)prop << std::endl;
        
        delete prop;
    }
napi_value ShaderProgramProp_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR){
        
        napi_status status;
        napi_value constructor, object, pointer_arg;

        status = napi_get_reference_value(env, ShaderProgramProp_reference, &constructor);
        
        if(status != napi_ok)
            std::cout << "Unable to dereference ShaderProgramProp_reference" << std::endl;

        status = napi_create_external( env, hive_prop, nullptr, nullptr, &pointer_arg);

        if(status != napi_ok)
            std::cout << "Unable to create a raw pointer reference" << std::endl;
        
        status = napi_new_instance(env, constructor, 1, &pointer_arg, &object);

        if(status != napi_ok)
            std::cout << "Unable to create JS instance of ShaderProgramProp" << std::endl; 
        
        return object;
    }

    napi_value ShaderProgramProp_Constructor(napi_env env, napi_callback_info info){
        
        ShaderProgramProp * hive_prop;

        // Create JS object.
        napi_status status;
        size_t arg_count = 1;
        napi_value this_arg, raw_arg;
        napi_valuetype arg_type;
        bool WRAP_EXISTING_POINTER = false;

        status = napi_get_cb_info(env, info, &arg_count, &raw_arg, &this_arg, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to get callback info for ShaderProgramProp_Constructor" << std::endl;
        
        if(arg_count == 1){

            status = napi_typeof(env, raw_arg, &arg_type);

            if(status != napi_ok)
                std::cout << "Unable to get callback info for ShaderProgramProp_Constructor" << std::endl;
            else if(arg_type == napi_external){
                void * ptr;

                status = napi_get_value_external(env, raw_arg, &ptr);

                if(status == napi_ok){   

                    WRAP_EXISTING_POINTER = true;

                    hive_prop = static_cast<ShaderProgramProp *>(ptr);
                    
                    std::cout << "wrapping instance of ShaderProgramProp " << (long long)hive_prop << std::endl;
                }
            }
        }
        
        if(!WRAP_EXISTING_POINTER){
            hive_prop = ShaderProgramProp::construct();

            std::cout << "creating instance of ShaderProgramProp " << (long long)hive_prop << std::endl;
        }

        if(WRAP_EXISTING_POINTER)
            status = napi_wrap(env, this_arg, hive_prop, nullptr, nullptr, nullptr);
        else
            status = napi_wrap(env, this_arg, hive_prop, ShaderProgramProp_Destructor, nullptr, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to wrap info for ShaderProgramProp_Constructor" << std::endl;


        return this_arg;
    }

        void ShaderProgramProp_Register(napi_env env, napi_value property_registry){
            
            napi_status status;
            
            napi_value result;

            status = napi_define_class(env, "ShaderProgramProp", NAPI_AUTO_LENGTH, &ShaderProgramProp_Constructor,
                nullptr, 10, ShaderProgramProp_props, &result);

            status = napi_set_named_property(env, property_registry, "ShaderProgramProp", result);

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE  CLASS CONSTRUCTOR FOR ShaderProgramProp" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR ShaderProgramProp" << std::endl;
            }

            status = napi_create_reference(env, result, 1, &ShaderProgramProp_reference);


        

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE REFERENCE FOR ShaderProgramProp" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR ShaderProgramProp" << std::endl;
            }
        }
        
        void RegisterInterfaces(napi_env env, napi_value registry){
            Prop_Register(env, registry);
SpriteProp_Register(env, registry);
Drone_Register(env, registry);
MeshProp_Register(env, registry);
ShaderProgramProp_Register(env, registry);

        }
    }
}