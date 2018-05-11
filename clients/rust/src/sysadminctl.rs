// This file is generated. Do not edit
// @generated

// https://github.com/Manishearth/rust-clippy/issues/702
#![allow(unknown_lints)]
#![allow(clippy)]

#![cfg_attr(rustfmt, rustfmt_skip)]

#![allow(box_pointers)]
#![allow(dead_code)]
#![allow(missing_docs)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(trivial_casts)]
#![allow(unsafe_code)]
#![allow(unused_imports)]
#![allow(unused_results)]

use protobuf::Message as Message_imported_for_functions;
use protobuf::ProtobufEnum as ProtobufEnum_imported_for_functions;

#[derive(PartialEq,Clone,Default)]
pub struct Int32List {
    // message fields
    list: ::std::vec::Vec<i32>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Int32List {}

impl Int32List {
    pub fn new() -> Int32List {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Int32List {
        static mut instance: ::protobuf::lazy::Lazy<Int32List> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Int32List,
        };
        unsafe {
            instance.get(Int32List::new)
        }
    }

    // repeated int32 list = 1;

    pub fn clear_list(&mut self) {
        self.list.clear();
    }

    // Param is passed by value, moved
    pub fn set_list(&mut self, v: ::std::vec::Vec<i32>) {
        self.list = v;
    }

    // Mutable pointer to the field.
    pub fn mut_list(&mut self) -> &mut ::std::vec::Vec<i32> {
        &mut self.list
    }

    // Take field
    pub fn take_list(&mut self) -> ::std::vec::Vec<i32> {
        ::std::mem::replace(&mut self.list, ::std::vec::Vec::new())
    }

    pub fn get_list(&self) -> &[i32] {
        &self.list
    }

    fn get_list_for_reflect(&self) -> &::std::vec::Vec<i32> {
        &self.list
    }

    fn mut_list_for_reflect(&mut self) -> &mut ::std::vec::Vec<i32> {
        &mut self.list
    }
}

impl ::protobuf::Message for Int32List {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_repeated_int32_into(wire_type, is, &mut self.list)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        for value in &self.list {
            my_size += ::protobuf::rt::value_size(1, *value, ::protobuf::wire_format::WireTypeVarint);
        };
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        for v in &self.list {
            os.write_int32(1, *v)?;
        };
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Int32List {
    fn new() -> Int32List {
        Int32List::new()
    }

    fn descriptor_static(_: ::std::option::Option<Int32List>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_vec_accessor::<_, ::protobuf::types::ProtobufTypeInt32>(
                    "list",
                    Int32List::get_list_for_reflect,
                    Int32List::mut_list_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<Int32List>(
                    "Int32List",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Int32List {
    fn clear(&mut self) {
        self.clear_list();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Int32List {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Int32List {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct BoolList {
    // message fields
    list: ::std::vec::Vec<bool>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for BoolList {}

impl BoolList {
    pub fn new() -> BoolList {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static BoolList {
        static mut instance: ::protobuf::lazy::Lazy<BoolList> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const BoolList,
        };
        unsafe {
            instance.get(BoolList::new)
        }
    }

    // repeated bool list = 1;

    pub fn clear_list(&mut self) {
        self.list.clear();
    }

    // Param is passed by value, moved
    pub fn set_list(&mut self, v: ::std::vec::Vec<bool>) {
        self.list = v;
    }

    // Mutable pointer to the field.
    pub fn mut_list(&mut self) -> &mut ::std::vec::Vec<bool> {
        &mut self.list
    }

    // Take field
    pub fn take_list(&mut self) -> ::std::vec::Vec<bool> {
        ::std::mem::replace(&mut self.list, ::std::vec::Vec::new())
    }

    pub fn get_list(&self) -> &[bool] {
        &self.list
    }

    fn get_list_for_reflect(&self) -> &::std::vec::Vec<bool> {
        &self.list
    }

    fn mut_list_for_reflect(&mut self) -> &mut ::std::vec::Vec<bool> {
        &mut self.list
    }
}

impl ::protobuf::Message for BoolList {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_repeated_bool_into(wire_type, is, &mut self.list)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        my_size += 2 * self.list.len() as u32;
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        for v in &self.list {
            os.write_bool(1, *v)?;
        };
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for BoolList {
    fn new() -> BoolList {
        BoolList::new()
    }

    fn descriptor_static(_: ::std::option::Option<BoolList>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_vec_accessor::<_, ::protobuf::types::ProtobufTypeBool>(
                    "list",
                    BoolList::get_list_for_reflect,
                    BoolList::mut_list_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<BoolList>(
                    "BoolList",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for BoolList {
    fn clear(&mut self) {
        self.clear_list();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for BoolList {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for BoolList {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct StringList {
    // message fields
    list: ::protobuf::RepeatedField<::std::string::String>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for StringList {}

impl StringList {
    pub fn new() -> StringList {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static StringList {
        static mut instance: ::protobuf::lazy::Lazy<StringList> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const StringList,
        };
        unsafe {
            instance.get(StringList::new)
        }
    }

    // repeated string list = 1;

    pub fn clear_list(&mut self) {
        self.list.clear();
    }

    // Param is passed by value, moved
    pub fn set_list(&mut self, v: ::protobuf::RepeatedField<::std::string::String>) {
        self.list = v;
    }

    // Mutable pointer to the field.
    pub fn mut_list(&mut self) -> &mut ::protobuf::RepeatedField<::std::string::String> {
        &mut self.list
    }

    // Take field
    pub fn take_list(&mut self) -> ::protobuf::RepeatedField<::std::string::String> {
        ::std::mem::replace(&mut self.list, ::protobuf::RepeatedField::new())
    }

    pub fn get_list(&self) -> &[::std::string::String] {
        &self.list
    }

    fn get_list_for_reflect(&self) -> &::protobuf::RepeatedField<::std::string::String> {
        &self.list
    }

    fn mut_list_for_reflect(&mut self) -> &mut ::protobuf::RepeatedField<::std::string::String> {
        &mut self.list
    }
}

impl ::protobuf::Message for StringList {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_repeated_string_into(wire_type, is, &mut self.list)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        for value in &self.list {
            my_size += ::protobuf::rt::string_size(1, &value);
        };
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        for v in &self.list {
            os.write_string(1, &v)?;
        };
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for StringList {
    fn new() -> StringList {
        StringList::new()
    }

    fn descriptor_static(_: ::std::option::Option<StringList>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_repeated_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "list",
                    StringList::get_list_for_reflect,
                    StringList::mut_list_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<StringList>(
                    "StringList",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for StringList {
    fn clear(&mut self) {
        self.clear_list();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for StringList {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for StringList {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct ConfigValue {
    // message oneof groups
    value: ::std::option::Option<ConfigValue_oneof_value>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for ConfigValue {}

#[derive(Clone,PartialEq)]
pub enum ConfigValue_oneof_value {
    int32val(i32),
    strval(::std::string::String),
    boolval(bool),
    int32list(Int32List),
    boollist(BoolList),
    stringlist(StringList),
}

impl ConfigValue {
    pub fn new() -> ConfigValue {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static ConfigValue {
        static mut instance: ::protobuf::lazy::Lazy<ConfigValue> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ConfigValue,
        };
        unsafe {
            instance.get(ConfigValue::new)
        }
    }

    // optional int32 int32val = 1;

    pub fn clear_int32val(&mut self) {
        self.value = ::std::option::Option::None;
    }

    pub fn has_int32val(&self) -> bool {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::int32val(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_int32val(&mut self, v: i32) {
        self.value = ::std::option::Option::Some(ConfigValue_oneof_value::int32val(v))
    }

    pub fn get_int32val(&self) -> i32 {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::int32val(v)) => v,
            _ => 0,
        }
    }

    // optional string strval = 2;

    pub fn clear_strval(&mut self) {
        self.value = ::std::option::Option::None;
    }

    pub fn has_strval(&self) -> bool {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::strval(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_strval(&mut self, v: ::std::string::String) {
        self.value = ::std::option::Option::Some(ConfigValue_oneof_value::strval(v))
    }

    // Mutable pointer to the field.
    pub fn mut_strval(&mut self) -> &mut ::std::string::String {
        if let ::std::option::Option::Some(ConfigValue_oneof_value::strval(_)) = self.value {
        } else {
            self.value = ::std::option::Option::Some(ConfigValue_oneof_value::strval(::std::string::String::new()));
        }
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::strval(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_strval(&mut self) -> ::std::string::String {
        if self.has_strval() {
            match self.value.take() {
                ::std::option::Option::Some(ConfigValue_oneof_value::strval(v)) => v,
                _ => panic!(),
            }
        } else {
            ::std::string::String::new()
        }
    }

    pub fn get_strval(&self) -> &str {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::strval(ref v)) => v,
            _ => "",
        }
    }

    // optional bool boolval = 3;

    pub fn clear_boolval(&mut self) {
        self.value = ::std::option::Option::None;
    }

    pub fn has_boolval(&self) -> bool {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::boolval(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_boolval(&mut self, v: bool) {
        self.value = ::std::option::Option::Some(ConfigValue_oneof_value::boolval(v))
    }

    pub fn get_boolval(&self) -> bool {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::boolval(v)) => v,
            _ => false,
        }
    }

    // optional .sysadminctl.Int32List int32list = 4;

    pub fn clear_int32list(&mut self) {
        self.value = ::std::option::Option::None;
    }

    pub fn has_int32list(&self) -> bool {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::int32list(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_int32list(&mut self, v: Int32List) {
        self.value = ::std::option::Option::Some(ConfigValue_oneof_value::int32list(v))
    }

    // Mutable pointer to the field.
    pub fn mut_int32list(&mut self) -> &mut Int32List {
        if let ::std::option::Option::Some(ConfigValue_oneof_value::int32list(_)) = self.value {
        } else {
            self.value = ::std::option::Option::Some(ConfigValue_oneof_value::int32list(Int32List::new()));
        }
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::int32list(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_int32list(&mut self) -> Int32List {
        if self.has_int32list() {
            match self.value.take() {
                ::std::option::Option::Some(ConfigValue_oneof_value::int32list(v)) => v,
                _ => panic!(),
            }
        } else {
            Int32List::new()
        }
    }

    pub fn get_int32list(&self) -> &Int32List {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::int32list(ref v)) => v,
            _ => Int32List::default_instance(),
        }
    }

    // optional .sysadminctl.BoolList boollist = 5;

    pub fn clear_boollist(&mut self) {
        self.value = ::std::option::Option::None;
    }

    pub fn has_boollist(&self) -> bool {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::boollist(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_boollist(&mut self, v: BoolList) {
        self.value = ::std::option::Option::Some(ConfigValue_oneof_value::boollist(v))
    }

    // Mutable pointer to the field.
    pub fn mut_boollist(&mut self) -> &mut BoolList {
        if let ::std::option::Option::Some(ConfigValue_oneof_value::boollist(_)) = self.value {
        } else {
            self.value = ::std::option::Option::Some(ConfigValue_oneof_value::boollist(BoolList::new()));
        }
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::boollist(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_boollist(&mut self) -> BoolList {
        if self.has_boollist() {
            match self.value.take() {
                ::std::option::Option::Some(ConfigValue_oneof_value::boollist(v)) => v,
                _ => panic!(),
            }
        } else {
            BoolList::new()
        }
    }

    pub fn get_boollist(&self) -> &BoolList {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::boollist(ref v)) => v,
            _ => BoolList::default_instance(),
        }
    }

    // optional .sysadminctl.StringList stringlist = 6;

    pub fn clear_stringlist(&mut self) {
        self.value = ::std::option::Option::None;
    }

    pub fn has_stringlist(&self) -> bool {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::stringlist(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_stringlist(&mut self, v: StringList) {
        self.value = ::std::option::Option::Some(ConfigValue_oneof_value::stringlist(v))
    }

    // Mutable pointer to the field.
    pub fn mut_stringlist(&mut self) -> &mut StringList {
        if let ::std::option::Option::Some(ConfigValue_oneof_value::stringlist(_)) = self.value {
        } else {
            self.value = ::std::option::Option::Some(ConfigValue_oneof_value::stringlist(StringList::new()));
        }
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::stringlist(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_stringlist(&mut self) -> StringList {
        if self.has_stringlist() {
            match self.value.take() {
                ::std::option::Option::Some(ConfigValue_oneof_value::stringlist(v)) => v,
                _ => panic!(),
            }
        } else {
            StringList::new()
        }
    }

    pub fn get_stringlist(&self) -> &StringList {
        match self.value {
            ::std::option::Option::Some(ConfigValue_oneof_value::stringlist(ref v)) => v,
            _ => StringList::default_instance(),
        }
    }
}

impl ::protobuf::Message for ConfigValue {
    fn is_initialized(&self) -> bool {
        if let Some(ConfigValue_oneof_value::int32list(ref v)) = self.value {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(ConfigValue_oneof_value::boollist(ref v)) = self.value {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(ConfigValue_oneof_value::stringlist(ref v)) = self.value {
            if !v.is_initialized() {
                return false;
            }
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.value = ::std::option::Option::Some(ConfigValue_oneof_value::int32val(is.read_int32()?));
                },
                2 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.value = ::std::option::Option::Some(ConfigValue_oneof_value::strval(is.read_string()?));
                },
                3 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.value = ::std::option::Option::Some(ConfigValue_oneof_value::boolval(is.read_bool()?));
                },
                4 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.value = ::std::option::Option::Some(ConfigValue_oneof_value::int32list(is.read_message()?));
                },
                5 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.value = ::std::option::Option::Some(ConfigValue_oneof_value::boollist(is.read_message()?));
                },
                6 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.value = ::std::option::Option::Some(ConfigValue_oneof_value::stringlist(is.read_message()?));
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let ::std::option::Option::Some(ref v) = self.value {
            match v {
                &ConfigValue_oneof_value::int32val(v) => {
                    my_size += ::protobuf::rt::value_size(1, v, ::protobuf::wire_format::WireTypeVarint);
                },
                &ConfigValue_oneof_value::strval(ref v) => {
                    my_size += ::protobuf::rt::string_size(2, &v);
                },
                &ConfigValue_oneof_value::boolval(v) => {
                    my_size += 2;
                },
                &ConfigValue_oneof_value::int32list(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &ConfigValue_oneof_value::boollist(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &ConfigValue_oneof_value::stringlist(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
            };
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let ::std::option::Option::Some(ref v) = self.value {
            match v {
                &ConfigValue_oneof_value::int32val(v) => {
                    os.write_int32(1, v)?;
                },
                &ConfigValue_oneof_value::strval(ref v) => {
                    os.write_string(2, v)?;
                },
                &ConfigValue_oneof_value::boolval(v) => {
                    os.write_bool(3, v)?;
                },
                &ConfigValue_oneof_value::int32list(ref v) => {
                    os.write_tag(4, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &ConfigValue_oneof_value::boollist(ref v) => {
                    os.write_tag(5, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &ConfigValue_oneof_value::stringlist(ref v) => {
                    os.write_tag(6, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
            };
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for ConfigValue {
    fn new() -> ConfigValue {
        ConfigValue::new()
    }

    fn descriptor_static(_: ::std::option::Option<ConfigValue>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_i32_accessor::<_>(
                    "int32val",
                    ConfigValue::has_int32val,
                    ConfigValue::get_int32val,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_string_accessor::<_>(
                    "strval",
                    ConfigValue::has_strval,
                    ConfigValue::get_strval,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_bool_accessor::<_>(
                    "boolval",
                    ConfigValue::has_boolval,
                    ConfigValue::get_boolval,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, Int32List>(
                    "int32list",
                    ConfigValue::has_int32list,
                    ConfigValue::get_int32list,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, BoolList>(
                    "boollist",
                    ConfigValue::has_boollist,
                    ConfigValue::get_boollist,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, StringList>(
                    "stringlist",
                    ConfigValue::has_stringlist,
                    ConfigValue::get_stringlist,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<ConfigValue>(
                    "ConfigValue",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for ConfigValue {
    fn clear(&mut self) {
        self.clear_int32val();
        self.clear_strval();
        self.clear_boolval();
        self.clear_int32list();
        self.clear_boollist();
        self.clear_stringlist();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for ConfigValue {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for ConfigValue {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Set {
    // message fields
    key: ::protobuf::SingularField<::std::string::String>,
    value: ::protobuf::SingularPtrField<ConfigValue>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Set {}

impl Set {
    pub fn new() -> Set {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Set {
        static mut instance: ::protobuf::lazy::Lazy<Set> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Set,
        };
        unsafe {
            instance.get(Set::new)
        }
    }

    // required string key = 1;

    pub fn clear_key(&mut self) {
        self.key.clear();
    }

    pub fn has_key(&self) -> bool {
        self.key.is_some()
    }

    // Param is passed by value, moved
    pub fn set_key(&mut self, v: ::std::string::String) {
        self.key = ::protobuf::SingularField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_key(&mut self) -> &mut ::std::string::String {
        if self.key.is_none() {
            self.key.set_default();
        }
        self.key.as_mut().unwrap()
    }

    // Take field
    pub fn take_key(&mut self) -> ::std::string::String {
        self.key.take().unwrap_or_else(|| ::std::string::String::new())
    }

    pub fn get_key(&self) -> &str {
        match self.key.as_ref() {
            Some(v) => &v,
            None => "",
        }
    }

    fn get_key_for_reflect(&self) -> &::protobuf::SingularField<::std::string::String> {
        &self.key
    }

    fn mut_key_for_reflect(&mut self) -> &mut ::protobuf::SingularField<::std::string::String> {
        &mut self.key
    }

    // required .sysadminctl.ConfigValue value = 2;

    pub fn clear_value(&mut self) {
        self.value.clear();
    }

    pub fn has_value(&self) -> bool {
        self.value.is_some()
    }

    // Param is passed by value, moved
    pub fn set_value(&mut self, v: ConfigValue) {
        self.value = ::protobuf::SingularPtrField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_value(&mut self) -> &mut ConfigValue {
        if self.value.is_none() {
            self.value.set_default();
        }
        self.value.as_mut().unwrap()
    }

    // Take field
    pub fn take_value(&mut self) -> ConfigValue {
        self.value.take().unwrap_or_else(|| ConfigValue::new())
    }

    pub fn get_value(&self) -> &ConfigValue {
        self.value.as_ref().unwrap_or_else(|| ConfigValue::default_instance())
    }

    fn get_value_for_reflect(&self) -> &::protobuf::SingularPtrField<ConfigValue> {
        &self.value
    }

    fn mut_value_for_reflect(&mut self) -> &mut ::protobuf::SingularPtrField<ConfigValue> {
        &mut self.value
    }
}

impl ::protobuf::Message for Set {
    fn is_initialized(&self) -> bool {
        if self.key.is_none() {
            return false;
        }
        if self.value.is_none() {
            return false;
        }
        for v in &self.value {
            if !v.is_initialized() {
                return false;
            }
        };
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_singular_string_into(wire_type, is, &mut self.key)?;
                },
                2 => {
                    ::protobuf::rt::read_singular_message_into(wire_type, is, &mut self.value)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(ref v) = self.key.as_ref() {
            my_size += ::protobuf::rt::string_size(1, &v);
        }
        if let Some(ref v) = self.value.as_ref() {
            let len = v.compute_size();
            my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(ref v) = self.key.as_ref() {
            os.write_string(1, &v)?;
        }
        if let Some(ref v) = self.value.as_ref() {
            os.write_tag(2, ::protobuf::wire_format::WireTypeLengthDelimited)?;
            os.write_raw_varint32(v.get_cached_size())?;
            v.write_to_with_cached_sizes(os)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Set {
    fn new() -> Set {
        Set::new()
    }

    fn descriptor_static(_: ::std::option::Option<Set>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "key",
                    Set::get_key_for_reflect,
                    Set::mut_key_for_reflect,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_ptr_field_accessor::<_, ::protobuf::types::ProtobufTypeMessage<ConfigValue>>(
                    "value",
                    Set::get_value_for_reflect,
                    Set::mut_value_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<Set>(
                    "Set",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Set {
    fn clear(&mut self) {
        self.clear_key();
        self.clear_value();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Set {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Set {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Get {
    // message fields
    key: ::protobuf::SingularField<::std::string::String>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Get {}

impl Get {
    pub fn new() -> Get {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Get {
        static mut instance: ::protobuf::lazy::Lazy<Get> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Get,
        };
        unsafe {
            instance.get(Get::new)
        }
    }

    // required string key = 1;

    pub fn clear_key(&mut self) {
        self.key.clear();
    }

    pub fn has_key(&self) -> bool {
        self.key.is_some()
    }

    // Param is passed by value, moved
    pub fn set_key(&mut self, v: ::std::string::String) {
        self.key = ::protobuf::SingularField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_key(&mut self) -> &mut ::std::string::String {
        if self.key.is_none() {
            self.key.set_default();
        }
        self.key.as_mut().unwrap()
    }

    // Take field
    pub fn take_key(&mut self) -> ::std::string::String {
        self.key.take().unwrap_or_else(|| ::std::string::String::new())
    }

    pub fn get_key(&self) -> &str {
        match self.key.as_ref() {
            Some(v) => &v,
            None => "",
        }
    }

    fn get_key_for_reflect(&self) -> &::protobuf::SingularField<::std::string::String> {
        &self.key
    }

    fn mut_key_for_reflect(&mut self) -> &mut ::protobuf::SingularField<::std::string::String> {
        &mut self.key
    }
}

impl ::protobuf::Message for Get {
    fn is_initialized(&self) -> bool {
        if self.key.is_none() {
            return false;
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_singular_string_into(wire_type, is, &mut self.key)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(ref v) = self.key.as_ref() {
            my_size += ::protobuf::rt::string_size(1, &v);
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(ref v) = self.key.as_ref() {
            os.write_string(1, &v)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Get {
    fn new() -> Get {
        Get::new()
    }

    fn descriptor_static(_: ::std::option::Option<Get>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "key",
                    Get::get_key_for_reflect,
                    Get::mut_key_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<Get>(
                    "Get",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Get {
    fn clear(&mut self) {
        self.clear_key();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Get {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Get {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Commit {
    // message fields
    config: ::std::option::Option<CommitConfig>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Commit {}

impl Commit {
    pub fn new() -> Commit {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Commit {
        static mut instance: ::protobuf::lazy::Lazy<Commit> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Commit,
        };
        unsafe {
            instance.get(Commit::new)
        }
    }

    // optional .sysadminctl.CommitConfig config = 1;

    pub fn clear_config(&mut self) {
        self.config = ::std::option::Option::None;
    }

    pub fn has_config(&self) -> bool {
        self.config.is_some()
    }

    // Param is passed by value, moved
    pub fn set_config(&mut self, v: CommitConfig) {
        self.config = ::std::option::Option::Some(v);
    }

    pub fn get_config(&self) -> CommitConfig {
        self.config.unwrap_or(CommitConfig::DEFAULT)
    }

    fn get_config_for_reflect(&self) -> &::std::option::Option<CommitConfig> {
        &self.config
    }

    fn mut_config_for_reflect(&mut self) -> &mut ::std::option::Option<CommitConfig> {
        &mut self.config
    }
}

impl ::protobuf::Message for Commit {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_enum()?;
                    self.config = ::std::option::Option::Some(tmp);
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(v) = self.config {
            my_size += ::protobuf::rt::enum_size(1, v);
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(v) = self.config {
            os.write_enum(1, v.value())?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Commit {
    fn new() -> Commit {
        Commit::new()
    }

    fn descriptor_static(_: ::std::option::Option<Commit>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeEnum<CommitConfig>>(
                    "config",
                    Commit::get_config_for_reflect,
                    Commit::mut_config_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<Commit>(
                    "Commit",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Commit {
    fn clear(&mut self) {
        self.clear_config();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Commit {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Commit {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Drop {
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Drop {}

impl Drop {
    pub fn new() -> Drop {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Drop {
        static mut instance: ::protobuf::lazy::Lazy<Drop> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Drop,
        };
        unsafe {
            instance.get(Drop::new)
        }
    }
}

impl ::protobuf::Message for Drop {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Drop {
    fn new() -> Drop {
        Drop::new()
    }

    fn descriptor_static(_: ::std::option::Option<Drop>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let fields = ::std::vec::Vec::new();
                ::protobuf::reflect::MessageDescriptor::new::<Drop>(
                    "Drop",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Drop {
    fn clear(&mut self) {
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Drop {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Drop {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct FireHooks {
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for FireHooks {}

impl FireHooks {
    pub fn new() -> FireHooks {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static FireHooks {
        static mut instance: ::protobuf::lazy::Lazy<FireHooks> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const FireHooks,
        };
        unsafe {
            instance.get(FireHooks::new)
        }
    }
}

impl ::protobuf::Message for FireHooks {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for FireHooks {
    fn new() -> FireHooks {
        FireHooks::new()
    }

    fn descriptor_static(_: ::std::option::Option<FireHooks>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let fields = ::std::vec::Vec::new();
                ::protobuf::reflect::MessageDescriptor::new::<FireHooks>(
                    "FireHooks",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for FireHooks {
    fn clear(&mut self) {
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for FireHooks {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for FireHooks {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Rollback {
    // message fields
    id: ::std::option::Option<u32>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Rollback {}

impl Rollback {
    pub fn new() -> Rollback {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Rollback {
        static mut instance: ::protobuf::lazy::Lazy<Rollback> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Rollback,
        };
        unsafe {
            instance.get(Rollback::new)
        }
    }

    // required uint32 id = 1;

    pub fn clear_id(&mut self) {
        self.id = ::std::option::Option::None;
    }

    pub fn has_id(&self) -> bool {
        self.id.is_some()
    }

    // Param is passed by value, moved
    pub fn set_id(&mut self, v: u32) {
        self.id = ::std::option::Option::Some(v);
    }

    pub fn get_id(&self) -> u32 {
        self.id.unwrap_or(0)
    }

    fn get_id_for_reflect(&self) -> &::std::option::Option<u32> {
        &self.id
    }

    fn mut_id_for_reflect(&mut self) -> &mut ::std::option::Option<u32> {
        &mut self.id
    }
}

impl ::protobuf::Message for Rollback {
    fn is_initialized(&self) -> bool {
        if self.id.is_none() {
            return false;
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_uint32()?;
                    self.id = ::std::option::Option::Some(tmp);
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(v) = self.id {
            my_size += ::protobuf::rt::value_size(1, v, ::protobuf::wire_format::WireTypeVarint);
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(v) = self.id {
            os.write_uint32(1, v)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Rollback {
    fn new() -> Rollback {
        Rollback::new()
    }

    fn descriptor_static(_: ::std::option::Option<Rollback>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeUint32>(
                    "id",
                    Rollback::get_id_for_reflect,
                    Rollback::mut_id_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<Rollback>(
                    "Rollback",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Rollback {
    fn clear(&mut self) {
        self.clear_id();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Rollback {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Rollback {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct EraseKey {
    // message fields
    key: ::protobuf::SingularField<::std::string::String>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for EraseKey {}

impl EraseKey {
    pub fn new() -> EraseKey {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static EraseKey {
        static mut instance: ::protobuf::lazy::Lazy<EraseKey> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const EraseKey,
        };
        unsafe {
            instance.get(EraseKey::new)
        }
    }

    // required string key = 1;

    pub fn clear_key(&mut self) {
        self.key.clear();
    }

    pub fn has_key(&self) -> bool {
        self.key.is_some()
    }

    // Param is passed by value, moved
    pub fn set_key(&mut self, v: ::std::string::String) {
        self.key = ::protobuf::SingularField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_key(&mut self) -> &mut ::std::string::String {
        if self.key.is_none() {
            self.key.set_default();
        }
        self.key.as_mut().unwrap()
    }

    // Take field
    pub fn take_key(&mut self) -> ::std::string::String {
        self.key.take().unwrap_or_else(|| ::std::string::String::new())
    }

    pub fn get_key(&self) -> &str {
        match self.key.as_ref() {
            Some(v) => &v,
            None => "",
        }
    }

    fn get_key_for_reflect(&self) -> &::protobuf::SingularField<::std::string::String> {
        &self.key
    }

    fn mut_key_for_reflect(&mut self) -> &mut ::protobuf::SingularField<::std::string::String> {
        &mut self.key
    }
}

impl ::protobuf::Message for EraseKey {
    fn is_initialized(&self) -> bool {
        if self.key.is_none() {
            return false;
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_singular_string_into(wire_type, is, &mut self.key)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(ref v) = self.key.as_ref() {
            my_size += ::protobuf::rt::string_size(1, &v);
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(ref v) = self.key.as_ref() {
            os.write_string(1, &v)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for EraseKey {
    fn new() -> EraseKey {
        EraseKey::new()
    }

    fn descriptor_static(_: ::std::option::Option<EraseKey>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "key",
                    EraseKey::get_key_for_reflect,
                    EraseKey::mut_key_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<EraseKey>(
                    "EraseKey",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for EraseKey {
    fn clear(&mut self) {
        self.clear_key();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for EraseKey {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for EraseKey {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Reset {
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Reset {}

impl Reset {
    pub fn new() -> Reset {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Reset {
        static mut instance: ::protobuf::lazy::Lazy<Reset> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Reset,
        };
        unsafe {
            instance.get(Reset::new)
        }
    }
}

impl ::protobuf::Message for Reset {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Reset {
    fn new() -> Reset {
        Reset::new()
    }

    fn descriptor_static(_: ::std::option::Option<Reset>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let fields = ::std::vec::Vec::new();
                ::protobuf::reflect::MessageDescriptor::new::<Reset>(
                    "Reset",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Reset {
    fn clear(&mut self) {
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Reset {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Reset {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct DumpHooks {
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for DumpHooks {}

impl DumpHooks {
    pub fn new() -> DumpHooks {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static DumpHooks {
        static mut instance: ::protobuf::lazy::Lazy<DumpHooks> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const DumpHooks,
        };
        unsafe {
            instance.get(DumpHooks::new)
        }
    }
}

impl ::protobuf::Message for DumpHooks {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for DumpHooks {
    fn new() -> DumpHooks {
        DumpHooks::new()
    }

    fn descriptor_static(_: ::std::option::Option<DumpHooks>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let fields = ::std::vec::Vec::new();
                ::protobuf::reflect::MessageDescriptor::new::<DumpHooks>(
                    "DumpHooks",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for DumpHooks {
    fn clear(&mut self) {
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for DumpHooks {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for DumpHooks {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct TriggerHook {
    // message fields
    hook: ::protobuf::SingularField<::std::string::String>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for TriggerHook {}

impl TriggerHook {
    pub fn new() -> TriggerHook {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static TriggerHook {
        static mut instance: ::protobuf::lazy::Lazy<TriggerHook> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const TriggerHook,
        };
        unsafe {
            instance.get(TriggerHook::new)
        }
    }

    // required string hook = 1;

    pub fn clear_hook(&mut self) {
        self.hook.clear();
    }

    pub fn has_hook(&self) -> bool {
        self.hook.is_some()
    }

    // Param is passed by value, moved
    pub fn set_hook(&mut self, v: ::std::string::String) {
        self.hook = ::protobuf::SingularField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_hook(&mut self) -> &mut ::std::string::String {
        if self.hook.is_none() {
            self.hook.set_default();
        }
        self.hook.as_mut().unwrap()
    }

    // Take field
    pub fn take_hook(&mut self) -> ::std::string::String {
        self.hook.take().unwrap_or_else(|| ::std::string::String::new())
    }

    pub fn get_hook(&self) -> &str {
        match self.hook.as_ref() {
            Some(v) => &v,
            None => "",
        }
    }

    fn get_hook_for_reflect(&self) -> &::protobuf::SingularField<::std::string::String> {
        &self.hook
    }

    fn mut_hook_for_reflect(&mut self) -> &mut ::protobuf::SingularField<::std::string::String> {
        &mut self.hook
    }
}

impl ::protobuf::Message for TriggerHook {
    fn is_initialized(&self) -> bool {
        if self.hook.is_none() {
            return false;
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_singular_string_into(wire_type, is, &mut self.hook)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(ref v) = self.hook.as_ref() {
            my_size += ::protobuf::rt::string_size(1, &v);
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(ref v) = self.hook.as_ref() {
            os.write_string(1, &v)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for TriggerHook {
    fn new() -> TriggerHook {
        TriggerHook::new()
    }

    fn descriptor_static(_: ::std::option::Option<TriggerHook>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "hook",
                    TriggerHook::get_hook_for_reflect,
                    TriggerHook::mut_hook_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<TriggerHook>(
                    "TriggerHook",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for TriggerHook {
    fn clear(&mut self) {
        self.clear_hook();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for TriggerHook {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for TriggerHook {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Blame {
    // message fields
    key: ::protobuf::SingularField<::std::string::String>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Blame {}

impl Blame {
    pub fn new() -> Blame {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Blame {
        static mut instance: ::protobuf::lazy::Lazy<Blame> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Blame,
        };
        unsafe {
            instance.get(Blame::new)
        }
    }

    // required string key = 1;

    pub fn clear_key(&mut self) {
        self.key.clear();
    }

    pub fn has_key(&self) -> bool {
        self.key.is_some()
    }

    // Param is passed by value, moved
    pub fn set_key(&mut self, v: ::std::string::String) {
        self.key = ::protobuf::SingularField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_key(&mut self) -> &mut ::std::string::String {
        if self.key.is_none() {
            self.key.set_default();
        }
        self.key.as_mut().unwrap()
    }

    // Take field
    pub fn take_key(&mut self) -> ::std::string::String {
        self.key.take().unwrap_or_else(|| ::std::string::String::new())
    }

    pub fn get_key(&self) -> &str {
        match self.key.as_ref() {
            Some(v) => &v,
            None => "",
        }
    }

    fn get_key_for_reflect(&self) -> &::protobuf::SingularField<::std::string::String> {
        &self.key
    }

    fn mut_key_for_reflect(&mut self) -> &mut ::protobuf::SingularField<::std::string::String> {
        &mut self.key
    }
}

impl ::protobuf::Message for Blame {
    fn is_initialized(&self) -> bool {
        if self.key.is_none() {
            return false;
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_singular_string_into(wire_type, is, &mut self.key)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(ref v) = self.key.as_ref() {
            my_size += ::protobuf::rt::string_size(1, &v);
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(ref v) = self.key.as_ref() {
            os.write_string(1, &v)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Blame {
    fn new() -> Blame {
        Blame::new()
    }

    fn descriptor_static(_: ::std::option::Option<Blame>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "key",
                    Blame::get_key_for_reflect,
                    Blame::mut_key_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<Blame>(
                    "Blame",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Blame {
    fn clear(&mut self) {
        self.clear_key();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Blame {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Blame {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct InFlight {
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for InFlight {}

impl InFlight {
    pub fn new() -> InFlight {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static InFlight {
        static mut instance: ::protobuf::lazy::Lazy<InFlight> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const InFlight,
        };
        unsafe {
            instance.get(InFlight::new)
        }
    }
}

impl ::protobuf::Message for InFlight {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for InFlight {
    fn new() -> InFlight {
        InFlight::new()
    }

    fn descriptor_static(_: ::std::option::Option<InFlight>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let fields = ::std::vec::Vec::new();
                ::protobuf::reflect::MessageDescriptor::new::<InFlight>(
                    "InFlight",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for InFlight {
    fn clear(&mut self) {
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for InFlight {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for InFlight {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Command {
    // message fields
    id: ::std::option::Option<u32>,
    xid: ::std::option::Option<u32>,
    // message oneof groups
    payload: ::std::option::Option<Command_oneof_payload>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Command {}

#[derive(Clone,PartialEq)]
pub enum Command_oneof_payload {
    set(Set),
    get(Get),
    commit(Commit),
    drop(Drop),
    firehooks(FireHooks),
    erase(EraseKey),
    rollback(Rollback),
    reset(Reset),
    dump(DumpHooks),
    trigger(TriggerHook),
    blame(Blame),
    inflight(InFlight),
}

impl Command {
    pub fn new() -> Command {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Command {
        static mut instance: ::protobuf::lazy::Lazy<Command> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Command,
        };
        unsafe {
            instance.get(Command::new)
        }
    }

    // optional .sysadminctl.Set set = 1;

    pub fn clear_set(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_set(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::set(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_set(&mut self, v: Set) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::set(v))
    }

    // Mutable pointer to the field.
    pub fn mut_set(&mut self) -> &mut Set {
        if let ::std::option::Option::Some(Command_oneof_payload::set(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::set(Set::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::set(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_set(&mut self) -> Set {
        if self.has_set() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::set(v)) => v,
                _ => panic!(),
            }
        } else {
            Set::new()
        }
    }

    pub fn get_set(&self) -> &Set {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::set(ref v)) => v,
            _ => Set::default_instance(),
        }
    }

    // optional .sysadminctl.Get get = 2;

    pub fn clear_get(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_get(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::get(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_get(&mut self, v: Get) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::get(v))
    }

    // Mutable pointer to the field.
    pub fn mut_get(&mut self) -> &mut Get {
        if let ::std::option::Option::Some(Command_oneof_payload::get(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::get(Get::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::get(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_get(&mut self) -> Get {
        if self.has_get() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::get(v)) => v,
                _ => panic!(),
            }
        } else {
            Get::new()
        }
    }

    pub fn get_get(&self) -> &Get {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::get(ref v)) => v,
            _ => Get::default_instance(),
        }
    }

    // optional .sysadminctl.Commit commit = 3;

    pub fn clear_commit(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_commit(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::commit(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_commit(&mut self, v: Commit) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::commit(v))
    }

    // Mutable pointer to the field.
    pub fn mut_commit(&mut self) -> &mut Commit {
        if let ::std::option::Option::Some(Command_oneof_payload::commit(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::commit(Commit::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::commit(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_commit(&mut self) -> Commit {
        if self.has_commit() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::commit(v)) => v,
                _ => panic!(),
            }
        } else {
            Commit::new()
        }
    }

    pub fn get_commit(&self) -> &Commit {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::commit(ref v)) => v,
            _ => Commit::default_instance(),
        }
    }

    // optional .sysadminctl.Drop drop = 4;

    pub fn clear_drop(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_drop(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::drop(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_drop(&mut self, v: Drop) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::drop(v))
    }

    // Mutable pointer to the field.
    pub fn mut_drop(&mut self) -> &mut Drop {
        if let ::std::option::Option::Some(Command_oneof_payload::drop(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::drop(Drop::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::drop(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_drop(&mut self) -> Drop {
        if self.has_drop() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::drop(v)) => v,
                _ => panic!(),
            }
        } else {
            Drop::new()
        }
    }

    pub fn get_drop(&self) -> &Drop {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::drop(ref v)) => v,
            _ => Drop::default_instance(),
        }
    }

    // optional .sysadminctl.FireHooks firehooks = 7;

    pub fn clear_firehooks(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_firehooks(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::firehooks(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_firehooks(&mut self, v: FireHooks) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::firehooks(v))
    }

    // Mutable pointer to the field.
    pub fn mut_firehooks(&mut self) -> &mut FireHooks {
        if let ::std::option::Option::Some(Command_oneof_payload::firehooks(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::firehooks(FireHooks::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::firehooks(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_firehooks(&mut self) -> FireHooks {
        if self.has_firehooks() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::firehooks(v)) => v,
                _ => panic!(),
            }
        } else {
            FireHooks::new()
        }
    }

    pub fn get_firehooks(&self) -> &FireHooks {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::firehooks(ref v)) => v,
            _ => FireHooks::default_instance(),
        }
    }

    // optional .sysadminctl.EraseKey erase = 8;

    pub fn clear_erase(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_erase(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::erase(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_erase(&mut self, v: EraseKey) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::erase(v))
    }

    // Mutable pointer to the field.
    pub fn mut_erase(&mut self) -> &mut EraseKey {
        if let ::std::option::Option::Some(Command_oneof_payload::erase(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::erase(EraseKey::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::erase(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_erase(&mut self) -> EraseKey {
        if self.has_erase() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::erase(v)) => v,
                _ => panic!(),
            }
        } else {
            EraseKey::new()
        }
    }

    pub fn get_erase(&self) -> &EraseKey {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::erase(ref v)) => v,
            _ => EraseKey::default_instance(),
        }
    }

    // optional .sysadminctl.Rollback rollback = 11;

    pub fn clear_rollback(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_rollback(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::rollback(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_rollback(&mut self, v: Rollback) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::rollback(v))
    }

    // Mutable pointer to the field.
    pub fn mut_rollback(&mut self) -> &mut Rollback {
        if let ::std::option::Option::Some(Command_oneof_payload::rollback(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::rollback(Rollback::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::rollback(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_rollback(&mut self) -> Rollback {
        if self.has_rollback() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::rollback(v)) => v,
                _ => panic!(),
            }
        } else {
            Rollback::new()
        }
    }

    pub fn get_rollback(&self) -> &Rollback {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::rollback(ref v)) => v,
            _ => Rollback::default_instance(),
        }
    }

    // optional .sysadminctl.Reset reset = 12;

    pub fn clear_reset(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_reset(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::reset(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_reset(&mut self, v: Reset) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::reset(v))
    }

    // Mutable pointer to the field.
    pub fn mut_reset(&mut self) -> &mut Reset {
        if let ::std::option::Option::Some(Command_oneof_payload::reset(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::reset(Reset::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::reset(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_reset(&mut self) -> Reset {
        if self.has_reset() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::reset(v)) => v,
                _ => panic!(),
            }
        } else {
            Reset::new()
        }
    }

    pub fn get_reset(&self) -> &Reset {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::reset(ref v)) => v,
            _ => Reset::default_instance(),
        }
    }

    // optional .sysadminctl.DumpHooks dump = 13;

    pub fn clear_dump(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_dump(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::dump(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_dump(&mut self, v: DumpHooks) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::dump(v))
    }

    // Mutable pointer to the field.
    pub fn mut_dump(&mut self) -> &mut DumpHooks {
        if let ::std::option::Option::Some(Command_oneof_payload::dump(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::dump(DumpHooks::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::dump(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_dump(&mut self) -> DumpHooks {
        if self.has_dump() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::dump(v)) => v,
                _ => panic!(),
            }
        } else {
            DumpHooks::new()
        }
    }

    pub fn get_dump(&self) -> &DumpHooks {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::dump(ref v)) => v,
            _ => DumpHooks::default_instance(),
        }
    }

    // optional .sysadminctl.TriggerHook trigger = 14;

    pub fn clear_trigger(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_trigger(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::trigger(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_trigger(&mut self, v: TriggerHook) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::trigger(v))
    }

    // Mutable pointer to the field.
    pub fn mut_trigger(&mut self) -> &mut TriggerHook {
        if let ::std::option::Option::Some(Command_oneof_payload::trigger(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::trigger(TriggerHook::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::trigger(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_trigger(&mut self) -> TriggerHook {
        if self.has_trigger() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::trigger(v)) => v,
                _ => panic!(),
            }
        } else {
            TriggerHook::new()
        }
    }

    pub fn get_trigger(&self) -> &TriggerHook {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::trigger(ref v)) => v,
            _ => TriggerHook::default_instance(),
        }
    }

    // optional .sysadminctl.Blame blame = 15;

    pub fn clear_blame(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_blame(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::blame(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_blame(&mut self, v: Blame) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::blame(v))
    }

    // Mutable pointer to the field.
    pub fn mut_blame(&mut self) -> &mut Blame {
        if let ::std::option::Option::Some(Command_oneof_payload::blame(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::blame(Blame::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::blame(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_blame(&mut self) -> Blame {
        if self.has_blame() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::blame(v)) => v,
                _ => panic!(),
            }
        } else {
            Blame::new()
        }
    }

    pub fn get_blame(&self) -> &Blame {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::blame(ref v)) => v,
            _ => Blame::default_instance(),
        }
    }

    // optional .sysadminctl.InFlight inflight = 16;

    pub fn clear_inflight(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_inflight(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::inflight(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_inflight(&mut self, v: InFlight) {
        self.payload = ::std::option::Option::Some(Command_oneof_payload::inflight(v))
    }

    // Mutable pointer to the field.
    pub fn mut_inflight(&mut self) -> &mut InFlight {
        if let ::std::option::Option::Some(Command_oneof_payload::inflight(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Command_oneof_payload::inflight(InFlight::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::inflight(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_inflight(&mut self) -> InFlight {
        if self.has_inflight() {
            match self.payload.take() {
                ::std::option::Option::Some(Command_oneof_payload::inflight(v)) => v,
                _ => panic!(),
            }
        } else {
            InFlight::new()
        }
    }

    pub fn get_inflight(&self) -> &InFlight {
        match self.payload {
            ::std::option::Option::Some(Command_oneof_payload::inflight(ref v)) => v,
            _ => InFlight::default_instance(),
        }
    }

    // required uint32 id = 6;

    pub fn clear_id(&mut self) {
        self.id = ::std::option::Option::None;
    }

    pub fn has_id(&self) -> bool {
        self.id.is_some()
    }

    // Param is passed by value, moved
    pub fn set_id(&mut self, v: u32) {
        self.id = ::std::option::Option::Some(v);
    }

    pub fn get_id(&self) -> u32 {
        self.id.unwrap_or(0)
    }

    fn get_id_for_reflect(&self) -> &::std::option::Option<u32> {
        &self.id
    }

    fn mut_id_for_reflect(&mut self) -> &mut ::std::option::Option<u32> {
        &mut self.id
    }

    // optional uint32 xid = 10;

    pub fn clear_xid(&mut self) {
        self.xid = ::std::option::Option::None;
    }

    pub fn has_xid(&self) -> bool {
        self.xid.is_some()
    }

    // Param is passed by value, moved
    pub fn set_xid(&mut self, v: u32) {
        self.xid = ::std::option::Option::Some(v);
    }

    pub fn get_xid(&self) -> u32 {
        self.xid.unwrap_or(0)
    }

    fn get_xid_for_reflect(&self) -> &::std::option::Option<u32> {
        &self.xid
    }

    fn mut_xid_for_reflect(&mut self) -> &mut ::std::option::Option<u32> {
        &mut self.xid
    }
}

impl ::protobuf::Message for Command {
    fn is_initialized(&self) -> bool {
        if self.id.is_none() {
            return false;
        }
        if let Some(Command_oneof_payload::set(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::get(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::commit(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::drop(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::firehooks(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::erase(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::rollback(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::reset(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::dump(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::trigger(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::blame(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Command_oneof_payload::inflight(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::set(is.read_message()?));
                },
                2 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::get(is.read_message()?));
                },
                3 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::commit(is.read_message()?));
                },
                4 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::drop(is.read_message()?));
                },
                7 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::firehooks(is.read_message()?));
                },
                8 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::erase(is.read_message()?));
                },
                11 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::rollback(is.read_message()?));
                },
                12 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::reset(is.read_message()?));
                },
                13 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::dump(is.read_message()?));
                },
                14 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::trigger(is.read_message()?));
                },
                15 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::blame(is.read_message()?));
                },
                16 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Command_oneof_payload::inflight(is.read_message()?));
                },
                6 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_uint32()?;
                    self.id = ::std::option::Option::Some(tmp);
                },
                10 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_uint32()?;
                    self.xid = ::std::option::Option::Some(tmp);
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(v) = self.id {
            my_size += ::protobuf::rt::value_size(6, v, ::protobuf::wire_format::WireTypeVarint);
        }
        if let Some(v) = self.xid {
            my_size += ::protobuf::rt::value_size(10, v, ::protobuf::wire_format::WireTypeVarint);
        }
        if let ::std::option::Option::Some(ref v) = self.payload {
            match v {
                &Command_oneof_payload::set(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::get(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::commit(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::drop(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::firehooks(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::erase(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::rollback(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::reset(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::dump(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::trigger(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::blame(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Command_oneof_payload::inflight(ref v) => {
                    let len = v.compute_size();
                    my_size += 2 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
            };
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(v) = self.id {
            os.write_uint32(6, v)?;
        }
        if let Some(v) = self.xid {
            os.write_uint32(10, v)?;
        }
        if let ::std::option::Option::Some(ref v) = self.payload {
            match v {
                &Command_oneof_payload::set(ref v) => {
                    os.write_tag(1, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::get(ref v) => {
                    os.write_tag(2, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::commit(ref v) => {
                    os.write_tag(3, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::drop(ref v) => {
                    os.write_tag(4, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::firehooks(ref v) => {
                    os.write_tag(7, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::erase(ref v) => {
                    os.write_tag(8, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::rollback(ref v) => {
                    os.write_tag(11, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::reset(ref v) => {
                    os.write_tag(12, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::dump(ref v) => {
                    os.write_tag(13, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::trigger(ref v) => {
                    os.write_tag(14, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::blame(ref v) => {
                    os.write_tag(15, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Command_oneof_payload::inflight(ref v) => {
                    os.write_tag(16, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
            };
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Command {
    fn new() -> Command {
        Command::new()
    }

    fn descriptor_static(_: ::std::option::Option<Command>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, Set>(
                    "set",
                    Command::has_set,
                    Command::get_set,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, Get>(
                    "get",
                    Command::has_get,
                    Command::get_get,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, Commit>(
                    "commit",
                    Command::has_commit,
                    Command::get_commit,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, Drop>(
                    "drop",
                    Command::has_drop,
                    Command::get_drop,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, FireHooks>(
                    "firehooks",
                    Command::has_firehooks,
                    Command::get_firehooks,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, EraseKey>(
                    "erase",
                    Command::has_erase,
                    Command::get_erase,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, Rollback>(
                    "rollback",
                    Command::has_rollback,
                    Command::get_rollback,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, Reset>(
                    "reset",
                    Command::has_reset,
                    Command::get_reset,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, DumpHooks>(
                    "dump",
                    Command::has_dump,
                    Command::get_dump,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, TriggerHook>(
                    "trigger",
                    Command::has_trigger,
                    Command::get_trigger,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, Blame>(
                    "blame",
                    Command::has_blame,
                    Command::get_blame,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, InFlight>(
                    "inflight",
                    Command::has_inflight,
                    Command::get_inflight,
                ));
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeUint32>(
                    "id",
                    Command::get_id_for_reflect,
                    Command::mut_id_for_reflect,
                ));
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeUint32>(
                    "xid",
                    Command::get_xid_for_reflect,
                    Command::mut_xid_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<Command>(
                    "Command",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Command {
    fn clear(&mut self) {
        self.clear_set();
        self.clear_get();
        self.clear_commit();
        self.clear_drop();
        self.clear_firehooks();
        self.clear_erase();
        self.clear_rollback();
        self.clear_reset();
        self.clear_dump();
        self.clear_trigger();
        self.clear_blame();
        self.clear_inflight();
        self.clear_id();
        self.clear_xid();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Command {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Command {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct MappedField {
    // message fields
    key: ::protobuf::SingularField<::std::string::String>,
    value: ::protobuf::SingularPtrField<ConfigValue>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for MappedField {}

impl MappedField {
    pub fn new() -> MappedField {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static MappedField {
        static mut instance: ::protobuf::lazy::Lazy<MappedField> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const MappedField,
        };
        unsafe {
            instance.get(MappedField::new)
        }
    }

    // required string key = 1;

    pub fn clear_key(&mut self) {
        self.key.clear();
    }

    pub fn has_key(&self) -> bool {
        self.key.is_some()
    }

    // Param is passed by value, moved
    pub fn set_key(&mut self, v: ::std::string::String) {
        self.key = ::protobuf::SingularField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_key(&mut self) -> &mut ::std::string::String {
        if self.key.is_none() {
            self.key.set_default();
        }
        self.key.as_mut().unwrap()
    }

    // Take field
    pub fn take_key(&mut self) -> ::std::string::String {
        self.key.take().unwrap_or_else(|| ::std::string::String::new())
    }

    pub fn get_key(&self) -> &str {
        match self.key.as_ref() {
            Some(v) => &v,
            None => "",
        }
    }

    fn get_key_for_reflect(&self) -> &::protobuf::SingularField<::std::string::String> {
        &self.key
    }

    fn mut_key_for_reflect(&mut self) -> &mut ::protobuf::SingularField<::std::string::String> {
        &mut self.key
    }

    // optional .sysadminctl.ConfigValue value = 2;

    pub fn clear_value(&mut self) {
        self.value.clear();
    }

    pub fn has_value(&self) -> bool {
        self.value.is_some()
    }

    // Param is passed by value, moved
    pub fn set_value(&mut self, v: ConfigValue) {
        self.value = ::protobuf::SingularPtrField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_value(&mut self) -> &mut ConfigValue {
        if self.value.is_none() {
            self.value.set_default();
        }
        self.value.as_mut().unwrap()
    }

    // Take field
    pub fn take_value(&mut self) -> ConfigValue {
        self.value.take().unwrap_or_else(|| ConfigValue::new())
    }

    pub fn get_value(&self) -> &ConfigValue {
        self.value.as_ref().unwrap_or_else(|| ConfigValue::default_instance())
    }

    fn get_value_for_reflect(&self) -> &::protobuf::SingularPtrField<ConfigValue> {
        &self.value
    }

    fn mut_value_for_reflect(&mut self) -> &mut ::protobuf::SingularPtrField<ConfigValue> {
        &mut self.value
    }
}

impl ::protobuf::Message for MappedField {
    fn is_initialized(&self) -> bool {
        if self.key.is_none() {
            return false;
        }
        for v in &self.value {
            if !v.is_initialized() {
                return false;
            }
        };
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_singular_string_into(wire_type, is, &mut self.key)?;
                },
                2 => {
                    ::protobuf::rt::read_singular_message_into(wire_type, is, &mut self.value)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(ref v) = self.key.as_ref() {
            my_size += ::protobuf::rt::string_size(1, &v);
        }
        if let Some(ref v) = self.value.as_ref() {
            let len = v.compute_size();
            my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(ref v) = self.key.as_ref() {
            os.write_string(1, &v)?;
        }
        if let Some(ref v) = self.value.as_ref() {
            os.write_tag(2, ::protobuf::wire_format::WireTypeLengthDelimited)?;
            os.write_raw_varint32(v.get_cached_size())?;
            v.write_to_with_cached_sizes(os)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for MappedField {
    fn new() -> MappedField {
        MappedField::new()
    }

    fn descriptor_static(_: ::std::option::Option<MappedField>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "key",
                    MappedField::get_key_for_reflect,
                    MappedField::mut_key_for_reflect,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_ptr_field_accessor::<_, ::protobuf::types::ProtobufTypeMessage<ConfigValue>>(
                    "value",
                    MappedField::get_value_for_reflect,
                    MappedField::mut_value_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<MappedField>(
                    "MappedField",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for MappedField {
    fn clear(&mut self) {
        self.clear_key();
        self.clear_value();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for MappedField {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for MappedField {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct GetResponse {
    // message fields
    kvs: ::protobuf::RepeatedField<MappedField>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for GetResponse {}

impl GetResponse {
    pub fn new() -> GetResponse {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static GetResponse {
        static mut instance: ::protobuf::lazy::Lazy<GetResponse> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const GetResponse,
        };
        unsafe {
            instance.get(GetResponse::new)
        }
    }

    // repeated .sysadminctl.MappedField kvs = 1;

    pub fn clear_kvs(&mut self) {
        self.kvs.clear();
    }

    // Param is passed by value, moved
    pub fn set_kvs(&mut self, v: ::protobuf::RepeatedField<MappedField>) {
        self.kvs = v;
    }

    // Mutable pointer to the field.
    pub fn mut_kvs(&mut self) -> &mut ::protobuf::RepeatedField<MappedField> {
        &mut self.kvs
    }

    // Take field
    pub fn take_kvs(&mut self) -> ::protobuf::RepeatedField<MappedField> {
        ::std::mem::replace(&mut self.kvs, ::protobuf::RepeatedField::new())
    }

    pub fn get_kvs(&self) -> &[MappedField] {
        &self.kvs
    }

    fn get_kvs_for_reflect(&self) -> &::protobuf::RepeatedField<MappedField> {
        &self.kvs
    }

    fn mut_kvs_for_reflect(&mut self) -> &mut ::protobuf::RepeatedField<MappedField> {
        &mut self.kvs
    }
}

impl ::protobuf::Message for GetResponse {
    fn is_initialized(&self) -> bool {
        for v in &self.kvs {
            if !v.is_initialized() {
                return false;
            }
        };
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_repeated_message_into(wire_type, is, &mut self.kvs)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        for value in &self.kvs {
            let len = value.compute_size();
            my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
        };
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        for v in &self.kvs {
            os.write_tag(1, ::protobuf::wire_format::WireTypeLengthDelimited)?;
            os.write_raw_varint32(v.get_cached_size())?;
            v.write_to_with_cached_sizes(os)?;
        };
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for GetResponse {
    fn new() -> GetResponse {
        GetResponse::new()
    }

    fn descriptor_static(_: ::std::option::Option<GetResponse>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_repeated_field_accessor::<_, ::protobuf::types::ProtobufTypeMessage<MappedField>>(
                    "kvs",
                    GetResponse::get_kvs_for_reflect,
                    GetResponse::mut_kvs_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<GetResponse>(
                    "GetResponse",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for GetResponse {
    fn clear(&mut self) {
        self.clear_kvs();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for GetResponse {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for GetResponse {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct CommitResponse {
    // message fields
    commit_id: ::std::option::Option<u32>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for CommitResponse {}

impl CommitResponse {
    pub fn new() -> CommitResponse {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static CommitResponse {
        static mut instance: ::protobuf::lazy::Lazy<CommitResponse> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const CommitResponse,
        };
        unsafe {
            instance.get(CommitResponse::new)
        }
    }

    // required uint32 commit_id = 1;

    pub fn clear_commit_id(&mut self) {
        self.commit_id = ::std::option::Option::None;
    }

    pub fn has_commit_id(&self) -> bool {
        self.commit_id.is_some()
    }

    // Param is passed by value, moved
    pub fn set_commit_id(&mut self, v: u32) {
        self.commit_id = ::std::option::Option::Some(v);
    }

    pub fn get_commit_id(&self) -> u32 {
        self.commit_id.unwrap_or(0)
    }

    fn get_commit_id_for_reflect(&self) -> &::std::option::Option<u32> {
        &self.commit_id
    }

    fn mut_commit_id_for_reflect(&mut self) -> &mut ::std::option::Option<u32> {
        &mut self.commit_id
    }
}

impl ::protobuf::Message for CommitResponse {
    fn is_initialized(&self) -> bool {
        if self.commit_id.is_none() {
            return false;
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_uint32()?;
                    self.commit_id = ::std::option::Option::Some(tmp);
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(v) = self.commit_id {
            my_size += ::protobuf::rt::value_size(1, v, ::protobuf::wire_format::WireTypeVarint);
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(v) = self.commit_id {
            os.write_uint32(1, v)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for CommitResponse {
    fn new() -> CommitResponse {
        CommitResponse::new()
    }

    fn descriptor_static(_: ::std::option::Option<CommitResponse>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeUint32>(
                    "commit_id",
                    CommitResponse::get_commit_id_for_reflect,
                    CommitResponse::mut_commit_id_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<CommitResponse>(
                    "CommitResponse",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for CommitResponse {
    fn clear(&mut self) {
        self.clear_commit_id();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for CommitResponse {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for CommitResponse {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct ResetResponse {
    // message fields
    commit_id: ::std::option::Option<u32>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for ResetResponse {}

impl ResetResponse {
    pub fn new() -> ResetResponse {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static ResetResponse {
        static mut instance: ::protobuf::lazy::Lazy<ResetResponse> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ResetResponse,
        };
        unsafe {
            instance.get(ResetResponse::new)
        }
    }

    // optional uint32 commit_id = 1;

    pub fn clear_commit_id(&mut self) {
        self.commit_id = ::std::option::Option::None;
    }

    pub fn has_commit_id(&self) -> bool {
        self.commit_id.is_some()
    }

    // Param is passed by value, moved
    pub fn set_commit_id(&mut self, v: u32) {
        self.commit_id = ::std::option::Option::Some(v);
    }

    pub fn get_commit_id(&self) -> u32 {
        self.commit_id.unwrap_or(0)
    }

    fn get_commit_id_for_reflect(&self) -> &::std::option::Option<u32> {
        &self.commit_id
    }

    fn mut_commit_id_for_reflect(&mut self) -> &mut ::std::option::Option<u32> {
        &mut self.commit_id
    }
}

impl ::protobuf::Message for ResetResponse {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_uint32()?;
                    self.commit_id = ::std::option::Option::Some(tmp);
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(v) = self.commit_id {
            my_size += ::protobuf::rt::value_size(1, v, ::protobuf::wire_format::WireTypeVarint);
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(v) = self.commit_id {
            os.write_uint32(1, v)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for ResetResponse {
    fn new() -> ResetResponse {
        ResetResponse::new()
    }

    fn descriptor_static(_: ::std::option::Option<ResetResponse>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeUint32>(
                    "commit_id",
                    ResetResponse::get_commit_id_for_reflect,
                    ResetResponse::mut_commit_id_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<ResetResponse>(
                    "ResetResponse",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for ResetResponse {
    fn clear(&mut self) {
        self.clear_commit_id();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for ResetResponse {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for ResetResponse {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct DumpResponse {
    // message fields
    templatehooks: ::protobuf::RepeatedField<::std::string::String>,
    servicehooks: ::protobuf::RepeatedField<::std::string::String>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for DumpResponse {}

impl DumpResponse {
    pub fn new() -> DumpResponse {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static DumpResponse {
        static mut instance: ::protobuf::lazy::Lazy<DumpResponse> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const DumpResponse,
        };
        unsafe {
            instance.get(DumpResponse::new)
        }
    }

    // repeated string templatehooks = 1;

    pub fn clear_templatehooks(&mut self) {
        self.templatehooks.clear();
    }

    // Param is passed by value, moved
    pub fn set_templatehooks(&mut self, v: ::protobuf::RepeatedField<::std::string::String>) {
        self.templatehooks = v;
    }

    // Mutable pointer to the field.
    pub fn mut_templatehooks(&mut self) -> &mut ::protobuf::RepeatedField<::std::string::String> {
        &mut self.templatehooks
    }

    // Take field
    pub fn take_templatehooks(&mut self) -> ::protobuf::RepeatedField<::std::string::String> {
        ::std::mem::replace(&mut self.templatehooks, ::protobuf::RepeatedField::new())
    }

    pub fn get_templatehooks(&self) -> &[::std::string::String] {
        &self.templatehooks
    }

    fn get_templatehooks_for_reflect(&self) -> &::protobuf::RepeatedField<::std::string::String> {
        &self.templatehooks
    }

    fn mut_templatehooks_for_reflect(&mut self) -> &mut ::protobuf::RepeatedField<::std::string::String> {
        &mut self.templatehooks
    }

    // repeated string servicehooks = 2;

    pub fn clear_servicehooks(&mut self) {
        self.servicehooks.clear();
    }

    // Param is passed by value, moved
    pub fn set_servicehooks(&mut self, v: ::protobuf::RepeatedField<::std::string::String>) {
        self.servicehooks = v;
    }

    // Mutable pointer to the field.
    pub fn mut_servicehooks(&mut self) -> &mut ::protobuf::RepeatedField<::std::string::String> {
        &mut self.servicehooks
    }

    // Take field
    pub fn take_servicehooks(&mut self) -> ::protobuf::RepeatedField<::std::string::String> {
        ::std::mem::replace(&mut self.servicehooks, ::protobuf::RepeatedField::new())
    }

    pub fn get_servicehooks(&self) -> &[::std::string::String] {
        &self.servicehooks
    }

    fn get_servicehooks_for_reflect(&self) -> &::protobuf::RepeatedField<::std::string::String> {
        &self.servicehooks
    }

    fn mut_servicehooks_for_reflect(&mut self) -> &mut ::protobuf::RepeatedField<::std::string::String> {
        &mut self.servicehooks
    }
}

impl ::protobuf::Message for DumpResponse {
    fn is_initialized(&self) -> bool {
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_repeated_string_into(wire_type, is, &mut self.templatehooks)?;
                },
                2 => {
                    ::protobuf::rt::read_repeated_string_into(wire_type, is, &mut self.servicehooks)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        for value in &self.templatehooks {
            my_size += ::protobuf::rt::string_size(1, &value);
        };
        for value in &self.servicehooks {
            my_size += ::protobuf::rt::string_size(2, &value);
        };
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        for v in &self.templatehooks {
            os.write_string(1, &v)?;
        };
        for v in &self.servicehooks {
            os.write_string(2, &v)?;
        };
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for DumpResponse {
    fn new() -> DumpResponse {
        DumpResponse::new()
    }

    fn descriptor_static(_: ::std::option::Option<DumpResponse>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_repeated_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "templatehooks",
                    DumpResponse::get_templatehooks_for_reflect,
                    DumpResponse::mut_templatehooks_for_reflect,
                ));
                fields.push(::protobuf::reflect::accessor::make_repeated_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "servicehooks",
                    DumpResponse::get_servicehooks_for_reflect,
                    DumpResponse::mut_servicehooks_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<DumpResponse>(
                    "DumpResponse",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for DumpResponse {
    fn clear(&mut self) {
        self.clear_templatehooks();
        self.clear_servicehooks();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for DumpResponse {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for DumpResponse {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct BlameEntry {
    // message fields
    commit_id: ::std::option::Option<u32>,
    commit_time: ::protobuf::SingularField<::std::string::String>,
    val: ::protobuf::SingularPtrField<ConfigValue>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for BlameEntry {}

impl BlameEntry {
    pub fn new() -> BlameEntry {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static BlameEntry {
        static mut instance: ::protobuf::lazy::Lazy<BlameEntry> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const BlameEntry,
        };
        unsafe {
            instance.get(BlameEntry::new)
        }
    }

    // required uint32 commit_id = 1;

    pub fn clear_commit_id(&mut self) {
        self.commit_id = ::std::option::Option::None;
    }

    pub fn has_commit_id(&self) -> bool {
        self.commit_id.is_some()
    }

    // Param is passed by value, moved
    pub fn set_commit_id(&mut self, v: u32) {
        self.commit_id = ::std::option::Option::Some(v);
    }

    pub fn get_commit_id(&self) -> u32 {
        self.commit_id.unwrap_or(0)
    }

    fn get_commit_id_for_reflect(&self) -> &::std::option::Option<u32> {
        &self.commit_id
    }

    fn mut_commit_id_for_reflect(&mut self) -> &mut ::std::option::Option<u32> {
        &mut self.commit_id
    }

    // required string commit_time = 2;

    pub fn clear_commit_time(&mut self) {
        self.commit_time.clear();
    }

    pub fn has_commit_time(&self) -> bool {
        self.commit_time.is_some()
    }

    // Param is passed by value, moved
    pub fn set_commit_time(&mut self, v: ::std::string::String) {
        self.commit_time = ::protobuf::SingularField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_commit_time(&mut self) -> &mut ::std::string::String {
        if self.commit_time.is_none() {
            self.commit_time.set_default();
        }
        self.commit_time.as_mut().unwrap()
    }

    // Take field
    pub fn take_commit_time(&mut self) -> ::std::string::String {
        self.commit_time.take().unwrap_or_else(|| ::std::string::String::new())
    }

    pub fn get_commit_time(&self) -> &str {
        match self.commit_time.as_ref() {
            Some(v) => &v,
            None => "",
        }
    }

    fn get_commit_time_for_reflect(&self) -> &::protobuf::SingularField<::std::string::String> {
        &self.commit_time
    }

    fn mut_commit_time_for_reflect(&mut self) -> &mut ::protobuf::SingularField<::std::string::String> {
        &mut self.commit_time
    }

    // required .sysadminctl.ConfigValue val = 3;

    pub fn clear_val(&mut self) {
        self.val.clear();
    }

    pub fn has_val(&self) -> bool {
        self.val.is_some()
    }

    // Param is passed by value, moved
    pub fn set_val(&mut self, v: ConfigValue) {
        self.val = ::protobuf::SingularPtrField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_val(&mut self) -> &mut ConfigValue {
        if self.val.is_none() {
            self.val.set_default();
        }
        self.val.as_mut().unwrap()
    }

    // Take field
    pub fn take_val(&mut self) -> ConfigValue {
        self.val.take().unwrap_or_else(|| ConfigValue::new())
    }

    pub fn get_val(&self) -> &ConfigValue {
        self.val.as_ref().unwrap_or_else(|| ConfigValue::default_instance())
    }

    fn get_val_for_reflect(&self) -> &::protobuf::SingularPtrField<ConfigValue> {
        &self.val
    }

    fn mut_val_for_reflect(&mut self) -> &mut ::protobuf::SingularPtrField<ConfigValue> {
        &mut self.val
    }
}

impl ::protobuf::Message for BlameEntry {
    fn is_initialized(&self) -> bool {
        if self.commit_id.is_none() {
            return false;
        }
        if self.commit_time.is_none() {
            return false;
        }
        if self.val.is_none() {
            return false;
        }
        for v in &self.val {
            if !v.is_initialized() {
                return false;
            }
        };
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_uint32()?;
                    self.commit_id = ::std::option::Option::Some(tmp);
                },
                2 => {
                    ::protobuf::rt::read_singular_string_into(wire_type, is, &mut self.commit_time)?;
                },
                3 => {
                    ::protobuf::rt::read_singular_message_into(wire_type, is, &mut self.val)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(v) = self.commit_id {
            my_size += ::protobuf::rt::value_size(1, v, ::protobuf::wire_format::WireTypeVarint);
        }
        if let Some(ref v) = self.commit_time.as_ref() {
            my_size += ::protobuf::rt::string_size(2, &v);
        }
        if let Some(ref v) = self.val.as_ref() {
            let len = v.compute_size();
            my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(v) = self.commit_id {
            os.write_uint32(1, v)?;
        }
        if let Some(ref v) = self.commit_time.as_ref() {
            os.write_string(2, &v)?;
        }
        if let Some(ref v) = self.val.as_ref() {
            os.write_tag(3, ::protobuf::wire_format::WireTypeLengthDelimited)?;
            os.write_raw_varint32(v.get_cached_size())?;
            v.write_to_with_cached_sizes(os)?;
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for BlameEntry {
    fn new() -> BlameEntry {
        BlameEntry::new()
    }

    fn descriptor_static(_: ::std::option::Option<BlameEntry>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeUint32>(
                    "commit_id",
                    BlameEntry::get_commit_id_for_reflect,
                    BlameEntry::mut_commit_id_for_reflect,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "commit_time",
                    BlameEntry::get_commit_time_for_reflect,
                    BlameEntry::mut_commit_time_for_reflect,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_ptr_field_accessor::<_, ::protobuf::types::ProtobufTypeMessage<ConfigValue>>(
                    "val",
                    BlameEntry::get_val_for_reflect,
                    BlameEntry::mut_val_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<BlameEntry>(
                    "BlameEntry",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for BlameEntry {
    fn clear(&mut self) {
        self.clear_commit_id();
        self.clear_commit_time();
        self.clear_val();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for BlameEntry {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for BlameEntry {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct BlameResponse {
    // message fields
    key: ::protobuf::SingularField<::std::string::String>,
    entries: ::protobuf::RepeatedField<BlameEntry>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for BlameResponse {}

impl BlameResponse {
    pub fn new() -> BlameResponse {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static BlameResponse {
        static mut instance: ::protobuf::lazy::Lazy<BlameResponse> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const BlameResponse,
        };
        unsafe {
            instance.get(BlameResponse::new)
        }
    }

    // required string key = 1;

    pub fn clear_key(&mut self) {
        self.key.clear();
    }

    pub fn has_key(&self) -> bool {
        self.key.is_some()
    }

    // Param is passed by value, moved
    pub fn set_key(&mut self, v: ::std::string::String) {
        self.key = ::protobuf::SingularField::some(v);
    }

    // Mutable pointer to the field.
    // If field is not initialized, it is initialized with default value first.
    pub fn mut_key(&mut self) -> &mut ::std::string::String {
        if self.key.is_none() {
            self.key.set_default();
        }
        self.key.as_mut().unwrap()
    }

    // Take field
    pub fn take_key(&mut self) -> ::std::string::String {
        self.key.take().unwrap_or_else(|| ::std::string::String::new())
    }

    pub fn get_key(&self) -> &str {
        match self.key.as_ref() {
            Some(v) => &v,
            None => "",
        }
    }

    fn get_key_for_reflect(&self) -> &::protobuf::SingularField<::std::string::String> {
        &self.key
    }

    fn mut_key_for_reflect(&mut self) -> &mut ::protobuf::SingularField<::std::string::String> {
        &mut self.key
    }

    // repeated .sysadminctl.BlameEntry entries = 2;

    pub fn clear_entries(&mut self) {
        self.entries.clear();
    }

    // Param is passed by value, moved
    pub fn set_entries(&mut self, v: ::protobuf::RepeatedField<BlameEntry>) {
        self.entries = v;
    }

    // Mutable pointer to the field.
    pub fn mut_entries(&mut self) -> &mut ::protobuf::RepeatedField<BlameEntry> {
        &mut self.entries
    }

    // Take field
    pub fn take_entries(&mut self) -> ::protobuf::RepeatedField<BlameEntry> {
        ::std::mem::replace(&mut self.entries, ::protobuf::RepeatedField::new())
    }

    pub fn get_entries(&self) -> &[BlameEntry] {
        &self.entries
    }

    fn get_entries_for_reflect(&self) -> &::protobuf::RepeatedField<BlameEntry> {
        &self.entries
    }

    fn mut_entries_for_reflect(&mut self) -> &mut ::protobuf::RepeatedField<BlameEntry> {
        &mut self.entries
    }
}

impl ::protobuf::Message for BlameResponse {
    fn is_initialized(&self) -> bool {
        if self.key.is_none() {
            return false;
        }
        for v in &self.entries {
            if !v.is_initialized() {
                return false;
            }
        };
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_singular_string_into(wire_type, is, &mut self.key)?;
                },
                2 => {
                    ::protobuf::rt::read_repeated_message_into(wire_type, is, &mut self.entries)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(ref v) = self.key.as_ref() {
            my_size += ::protobuf::rt::string_size(1, &v);
        }
        for value in &self.entries {
            let len = value.compute_size();
            my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
        };
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(ref v) = self.key.as_ref() {
            os.write_string(1, &v)?;
        }
        for v in &self.entries {
            os.write_tag(2, ::protobuf::wire_format::WireTypeLengthDelimited)?;
            os.write_raw_varint32(v.get_cached_size())?;
            v.write_to_with_cached_sizes(os)?;
        };
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for BlameResponse {
    fn new() -> BlameResponse {
        BlameResponse::new()
    }

    fn descriptor_static(_: ::std::option::Option<BlameResponse>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_field_accessor::<_, ::protobuf::types::ProtobufTypeString>(
                    "key",
                    BlameResponse::get_key_for_reflect,
                    BlameResponse::mut_key_for_reflect,
                ));
                fields.push(::protobuf::reflect::accessor::make_repeated_field_accessor::<_, ::protobuf::types::ProtobufTypeMessage<BlameEntry>>(
                    "entries",
                    BlameResponse::get_entries_for_reflect,
                    BlameResponse::mut_entries_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<BlameResponse>(
                    "BlameResponse",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for BlameResponse {
    fn clear(&mut self) {
        self.clear_key();
        self.clear_entries();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for BlameResponse {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for BlameResponse {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct InFlightResponse {
    // message fields
    kvs: ::protobuf::RepeatedField<MappedField>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for InFlightResponse {}

impl InFlightResponse {
    pub fn new() -> InFlightResponse {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static InFlightResponse {
        static mut instance: ::protobuf::lazy::Lazy<InFlightResponse> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const InFlightResponse,
        };
        unsafe {
            instance.get(InFlightResponse::new)
        }
    }

    // repeated .sysadminctl.MappedField kvs = 1;

    pub fn clear_kvs(&mut self) {
        self.kvs.clear();
    }

    // Param is passed by value, moved
    pub fn set_kvs(&mut self, v: ::protobuf::RepeatedField<MappedField>) {
        self.kvs = v;
    }

    // Mutable pointer to the field.
    pub fn mut_kvs(&mut self) -> &mut ::protobuf::RepeatedField<MappedField> {
        &mut self.kvs
    }

    // Take field
    pub fn take_kvs(&mut self) -> ::protobuf::RepeatedField<MappedField> {
        ::std::mem::replace(&mut self.kvs, ::protobuf::RepeatedField::new())
    }

    pub fn get_kvs(&self) -> &[MappedField] {
        &self.kvs
    }

    fn get_kvs_for_reflect(&self) -> &::protobuf::RepeatedField<MappedField> {
        &self.kvs
    }

    fn mut_kvs_for_reflect(&mut self) -> &mut ::protobuf::RepeatedField<MappedField> {
        &mut self.kvs
    }
}

impl ::protobuf::Message for InFlightResponse {
    fn is_initialized(&self) -> bool {
        for v in &self.kvs {
            if !v.is_initialized() {
                return false;
            }
        };
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    ::protobuf::rt::read_repeated_message_into(wire_type, is, &mut self.kvs)?;
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        for value in &self.kvs {
            let len = value.compute_size();
            my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
        };
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        for v in &self.kvs {
            os.write_tag(1, ::protobuf::wire_format::WireTypeLengthDelimited)?;
            os.write_raw_varint32(v.get_cached_size())?;
            v.write_to_with_cached_sizes(os)?;
        };
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for InFlightResponse {
    fn new() -> InFlightResponse {
        InFlightResponse::new()
    }

    fn descriptor_static(_: ::std::option::Option<InFlightResponse>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_repeated_field_accessor::<_, ::protobuf::types::ProtobufTypeMessage<MappedField>>(
                    "kvs",
                    InFlightResponse::get_kvs_for_reflect,
                    InFlightResponse::mut_kvs_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<InFlightResponse>(
                    "InFlightResponse",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for InFlightResponse {
    fn clear(&mut self) {
        self.clear_kvs();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for InFlightResponse {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for InFlightResponse {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(PartialEq,Clone,Default)]
pub struct Response {
    // message fields
    id: ::std::option::Option<u32>,
    status: ::std::option::Option<StatusCode>,
    // message oneof groups
    payload: ::std::option::Option<Response_oneof_payload>,
    // special fields
    unknown_fields: ::protobuf::UnknownFields,
    cached_size: ::protobuf::CachedSize,
}

// see codegen.rs for the explanation why impl Sync explicitly
unsafe impl ::std::marker::Sync for Response {}

#[derive(Clone,PartialEq)]
pub enum Response_oneof_payload {
    get(GetResponse),
    commit(CommitResponse),
    reset(ResetResponse),
    dump(DumpResponse),
    blame(BlameResponse),
    inflight(InFlightResponse),
}

impl Response {
    pub fn new() -> Response {
        ::std::default::Default::default()
    }

    pub fn default_instance() -> &'static Response {
        static mut instance: ::protobuf::lazy::Lazy<Response> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const Response,
        };
        unsafe {
            instance.get(Response::new)
        }
    }

    // optional .sysadminctl.GetResponse get = 1;

    pub fn clear_get(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_get(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::get(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_get(&mut self, v: GetResponse) {
        self.payload = ::std::option::Option::Some(Response_oneof_payload::get(v))
    }

    // Mutable pointer to the field.
    pub fn mut_get(&mut self) -> &mut GetResponse {
        if let ::std::option::Option::Some(Response_oneof_payload::get(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Response_oneof_payload::get(GetResponse::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::get(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_get(&mut self) -> GetResponse {
        if self.has_get() {
            match self.payload.take() {
                ::std::option::Option::Some(Response_oneof_payload::get(v)) => v,
                _ => panic!(),
            }
        } else {
            GetResponse::new()
        }
    }

    pub fn get_get(&self) -> &GetResponse {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::get(ref v)) => v,
            _ => GetResponse::default_instance(),
        }
    }

    // optional .sysadminctl.CommitResponse commit = 2;

    pub fn clear_commit(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_commit(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::commit(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_commit(&mut self, v: CommitResponse) {
        self.payload = ::std::option::Option::Some(Response_oneof_payload::commit(v))
    }

    // Mutable pointer to the field.
    pub fn mut_commit(&mut self) -> &mut CommitResponse {
        if let ::std::option::Option::Some(Response_oneof_payload::commit(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Response_oneof_payload::commit(CommitResponse::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::commit(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_commit(&mut self) -> CommitResponse {
        if self.has_commit() {
            match self.payload.take() {
                ::std::option::Option::Some(Response_oneof_payload::commit(v)) => v,
                _ => panic!(),
            }
        } else {
            CommitResponse::new()
        }
    }

    pub fn get_commit(&self) -> &CommitResponse {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::commit(ref v)) => v,
            _ => CommitResponse::default_instance(),
        }
    }

    // optional .sysadminctl.ResetResponse reset = 3;

    pub fn clear_reset(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_reset(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::reset(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_reset(&mut self, v: ResetResponse) {
        self.payload = ::std::option::Option::Some(Response_oneof_payload::reset(v))
    }

    // Mutable pointer to the field.
    pub fn mut_reset(&mut self) -> &mut ResetResponse {
        if let ::std::option::Option::Some(Response_oneof_payload::reset(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Response_oneof_payload::reset(ResetResponse::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::reset(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_reset(&mut self) -> ResetResponse {
        if self.has_reset() {
            match self.payload.take() {
                ::std::option::Option::Some(Response_oneof_payload::reset(v)) => v,
                _ => panic!(),
            }
        } else {
            ResetResponse::new()
        }
    }

    pub fn get_reset(&self) -> &ResetResponse {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::reset(ref v)) => v,
            _ => ResetResponse::default_instance(),
        }
    }

    // optional .sysadminctl.DumpResponse dump = 4;

    pub fn clear_dump(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_dump(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::dump(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_dump(&mut self, v: DumpResponse) {
        self.payload = ::std::option::Option::Some(Response_oneof_payload::dump(v))
    }

    // Mutable pointer to the field.
    pub fn mut_dump(&mut self) -> &mut DumpResponse {
        if let ::std::option::Option::Some(Response_oneof_payload::dump(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Response_oneof_payload::dump(DumpResponse::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::dump(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_dump(&mut self) -> DumpResponse {
        if self.has_dump() {
            match self.payload.take() {
                ::std::option::Option::Some(Response_oneof_payload::dump(v)) => v,
                _ => panic!(),
            }
        } else {
            DumpResponse::new()
        }
    }

    pub fn get_dump(&self) -> &DumpResponse {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::dump(ref v)) => v,
            _ => DumpResponse::default_instance(),
        }
    }

    // optional .sysadminctl.BlameResponse blame = 5;

    pub fn clear_blame(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_blame(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::blame(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_blame(&mut self, v: BlameResponse) {
        self.payload = ::std::option::Option::Some(Response_oneof_payload::blame(v))
    }

    // Mutable pointer to the field.
    pub fn mut_blame(&mut self) -> &mut BlameResponse {
        if let ::std::option::Option::Some(Response_oneof_payload::blame(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Response_oneof_payload::blame(BlameResponse::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::blame(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_blame(&mut self) -> BlameResponse {
        if self.has_blame() {
            match self.payload.take() {
                ::std::option::Option::Some(Response_oneof_payload::blame(v)) => v,
                _ => panic!(),
            }
        } else {
            BlameResponse::new()
        }
    }

    pub fn get_blame(&self) -> &BlameResponse {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::blame(ref v)) => v,
            _ => BlameResponse::default_instance(),
        }
    }

    // optional .sysadminctl.InFlightResponse inflight = 8;

    pub fn clear_inflight(&mut self) {
        self.payload = ::std::option::Option::None;
    }

    pub fn has_inflight(&self) -> bool {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::inflight(..)) => true,
            _ => false,
        }
    }

    // Param is passed by value, moved
    pub fn set_inflight(&mut self, v: InFlightResponse) {
        self.payload = ::std::option::Option::Some(Response_oneof_payload::inflight(v))
    }

    // Mutable pointer to the field.
    pub fn mut_inflight(&mut self) -> &mut InFlightResponse {
        if let ::std::option::Option::Some(Response_oneof_payload::inflight(_)) = self.payload {
        } else {
            self.payload = ::std::option::Option::Some(Response_oneof_payload::inflight(InFlightResponse::new()));
        }
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::inflight(ref mut v)) => v,
            _ => panic!(),
        }
    }

    // Take field
    pub fn take_inflight(&mut self) -> InFlightResponse {
        if self.has_inflight() {
            match self.payload.take() {
                ::std::option::Option::Some(Response_oneof_payload::inflight(v)) => v,
                _ => panic!(),
            }
        } else {
            InFlightResponse::new()
        }
    }

    pub fn get_inflight(&self) -> &InFlightResponse {
        match self.payload {
            ::std::option::Option::Some(Response_oneof_payload::inflight(ref v)) => v,
            _ => InFlightResponse::default_instance(),
        }
    }

    // required uint32 id = 6;

    pub fn clear_id(&mut self) {
        self.id = ::std::option::Option::None;
    }

    pub fn has_id(&self) -> bool {
        self.id.is_some()
    }

    // Param is passed by value, moved
    pub fn set_id(&mut self, v: u32) {
        self.id = ::std::option::Option::Some(v);
    }

    pub fn get_id(&self) -> u32 {
        self.id.unwrap_or(0)
    }

    fn get_id_for_reflect(&self) -> &::std::option::Option<u32> {
        &self.id
    }

    fn mut_id_for_reflect(&mut self) -> &mut ::std::option::Option<u32> {
        &mut self.id
    }

    // required .sysadminctl.StatusCode status = 7;

    pub fn clear_status(&mut self) {
        self.status = ::std::option::Option::None;
    }

    pub fn has_status(&self) -> bool {
        self.status.is_some()
    }

    // Param is passed by value, moved
    pub fn set_status(&mut self, v: StatusCode) {
        self.status = ::std::option::Option::Some(v);
    }

    pub fn get_status(&self) -> StatusCode {
        self.status.unwrap_or(StatusCode::SUCCESS)
    }

    fn get_status_for_reflect(&self) -> &::std::option::Option<StatusCode> {
        &self.status
    }

    fn mut_status_for_reflect(&mut self) -> &mut ::std::option::Option<StatusCode> {
        &mut self.status
    }
}

impl ::protobuf::Message for Response {
    fn is_initialized(&self) -> bool {
        if self.id.is_none() {
            return false;
        }
        if self.status.is_none() {
            return false;
        }
        if let Some(Response_oneof_payload::get(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Response_oneof_payload::commit(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Response_oneof_payload::reset(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Response_oneof_payload::dump(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Response_oneof_payload::blame(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        if let Some(Response_oneof_payload::inflight(ref v)) = self.payload {
            if !v.is_initialized() {
                return false;
            }
        }
        true
    }

    fn merge_from(&mut self, is: &mut ::protobuf::CodedInputStream) -> ::protobuf::ProtobufResult<()> {
        while !is.eof()? {
            let (field_number, wire_type) = is.read_tag_unpack()?;
            match field_number {
                1 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Response_oneof_payload::get(is.read_message()?));
                },
                2 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Response_oneof_payload::commit(is.read_message()?));
                },
                3 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Response_oneof_payload::reset(is.read_message()?));
                },
                4 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Response_oneof_payload::dump(is.read_message()?));
                },
                5 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Response_oneof_payload::blame(is.read_message()?));
                },
                8 => {
                    if wire_type != ::protobuf::wire_format::WireTypeLengthDelimited {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    self.payload = ::std::option::Option::Some(Response_oneof_payload::inflight(is.read_message()?));
                },
                6 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_uint32()?;
                    self.id = ::std::option::Option::Some(tmp);
                },
                7 => {
                    if wire_type != ::protobuf::wire_format::WireTypeVarint {
                        return ::std::result::Result::Err(::protobuf::rt::unexpected_wire_type(wire_type));
                    }
                    let tmp = is.read_enum()?;
                    self.status = ::std::option::Option::Some(tmp);
                },
                _ => {
                    ::protobuf::rt::read_unknown_or_skip_group(field_number, wire_type, is, self.mut_unknown_fields())?;
                },
            };
        }
        ::std::result::Result::Ok(())
    }

    // Compute sizes of nested messages
    #[allow(unused_variables)]
    fn compute_size(&self) -> u32 {
        let mut my_size = 0;
        if let Some(v) = self.id {
            my_size += ::protobuf::rt::value_size(6, v, ::protobuf::wire_format::WireTypeVarint);
        }
        if let Some(v) = self.status {
            my_size += ::protobuf::rt::enum_size(7, v);
        }
        if let ::std::option::Option::Some(ref v) = self.payload {
            match v {
                &Response_oneof_payload::get(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Response_oneof_payload::commit(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Response_oneof_payload::reset(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Response_oneof_payload::dump(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Response_oneof_payload::blame(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
                &Response_oneof_payload::inflight(ref v) => {
                    let len = v.compute_size();
                    my_size += 1 + ::protobuf::rt::compute_raw_varint32_size(len) + len;
                },
            };
        }
        my_size += ::protobuf::rt::unknown_fields_size(self.get_unknown_fields());
        self.cached_size.set(my_size);
        my_size
    }

    fn write_to_with_cached_sizes(&self, os: &mut ::protobuf::CodedOutputStream) -> ::protobuf::ProtobufResult<()> {
        if let Some(v) = self.id {
            os.write_uint32(6, v)?;
        }
        if let Some(v) = self.status {
            os.write_enum(7, v.value())?;
        }
        if let ::std::option::Option::Some(ref v) = self.payload {
            match v {
                &Response_oneof_payload::get(ref v) => {
                    os.write_tag(1, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Response_oneof_payload::commit(ref v) => {
                    os.write_tag(2, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Response_oneof_payload::reset(ref v) => {
                    os.write_tag(3, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Response_oneof_payload::dump(ref v) => {
                    os.write_tag(4, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Response_oneof_payload::blame(ref v) => {
                    os.write_tag(5, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
                &Response_oneof_payload::inflight(ref v) => {
                    os.write_tag(8, ::protobuf::wire_format::WireTypeLengthDelimited)?;
                    os.write_raw_varint32(v.get_cached_size())?;
                    v.write_to_with_cached_sizes(os)?;
                },
            };
        }
        os.write_unknown_fields(self.get_unknown_fields())?;
        ::std::result::Result::Ok(())
    }

    fn get_cached_size(&self) -> u32 {
        self.cached_size.get()
    }

    fn get_unknown_fields(&self) -> &::protobuf::UnknownFields {
        &self.unknown_fields
    }

    fn mut_unknown_fields(&mut self) -> &mut ::protobuf::UnknownFields {
        &mut self.unknown_fields
    }

    fn as_any(&self) -> &::std::any::Any {
        self as &::std::any::Any
    }
    fn as_any_mut(&mut self) -> &mut ::std::any::Any {
        self as &mut ::std::any::Any
    }
    fn into_any(self: Box<Self>) -> ::std::boxed::Box<::std::any::Any> {
        self
    }

    fn descriptor(&self) -> &'static ::protobuf::reflect::MessageDescriptor {
        ::protobuf::MessageStatic::descriptor_static(None::<Self>)
    }
}

impl ::protobuf::MessageStatic for Response {
    fn new() -> Response {
        Response::new()
    }

    fn descriptor_static(_: ::std::option::Option<Response>) -> &'static ::protobuf::reflect::MessageDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::MessageDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::MessageDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                let mut fields = ::std::vec::Vec::new();
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, GetResponse>(
                    "get",
                    Response::has_get,
                    Response::get_get,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, CommitResponse>(
                    "commit",
                    Response::has_commit,
                    Response::get_commit,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, ResetResponse>(
                    "reset",
                    Response::has_reset,
                    Response::get_reset,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, DumpResponse>(
                    "dump",
                    Response::has_dump,
                    Response::get_dump,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, BlameResponse>(
                    "blame",
                    Response::has_blame,
                    Response::get_blame,
                ));
                fields.push(::protobuf::reflect::accessor::make_singular_message_accessor::<_, InFlightResponse>(
                    "inflight",
                    Response::has_inflight,
                    Response::get_inflight,
                ));
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeUint32>(
                    "id",
                    Response::get_id_for_reflect,
                    Response::mut_id_for_reflect,
                ));
                fields.push(::protobuf::reflect::accessor::make_option_accessor::<_, ::protobuf::types::ProtobufTypeEnum<StatusCode>>(
                    "status",
                    Response::get_status_for_reflect,
                    Response::mut_status_for_reflect,
                ));
                ::protobuf::reflect::MessageDescriptor::new::<Response>(
                    "Response",
                    fields,
                    file_descriptor_proto()
                )
            })
        }
    }
}

impl ::protobuf::Clear for Response {
    fn clear(&mut self) {
        self.clear_get();
        self.clear_commit();
        self.clear_reset();
        self.clear_dump();
        self.clear_blame();
        self.clear_inflight();
        self.clear_id();
        self.clear_status();
        self.unknown_fields.clear();
    }
}

impl ::std::fmt::Debug for Response {
    fn fmt(&self, f: &mut ::std::fmt::Formatter) -> ::std::fmt::Result {
        ::protobuf::text_format::fmt(self, f)
    }
}

impl ::protobuf::reflect::ProtobufValue for Response {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Message(self)
    }
}

#[derive(Clone,PartialEq,Eq,Debug,Hash)]
pub enum CommitConfig {
    DEFAULT = 0,
    TEMPLATE_ONLY = 1,
    NO_HOOKS = 3,
}

impl ::protobuf::ProtobufEnum for CommitConfig {
    fn value(&self) -> i32 {
        *self as i32
    }

    fn from_i32(value: i32) -> ::std::option::Option<CommitConfig> {
        match value {
            0 => ::std::option::Option::Some(CommitConfig::DEFAULT),
            1 => ::std::option::Option::Some(CommitConfig::TEMPLATE_ONLY),
            3 => ::std::option::Option::Some(CommitConfig::NO_HOOKS),
            _ => ::std::option::Option::None
        }
    }

    fn values() -> &'static [Self] {
        static values: &'static [CommitConfig] = &[
            CommitConfig::DEFAULT,
            CommitConfig::TEMPLATE_ONLY,
            CommitConfig::NO_HOOKS,
        ];
        values
    }

    fn enum_descriptor_static(_: ::std::option::Option<CommitConfig>) -> &'static ::protobuf::reflect::EnumDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::EnumDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::EnumDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                ::protobuf::reflect::EnumDescriptor::new("CommitConfig", file_descriptor_proto())
            })
        }
    }
}

impl ::std::marker::Copy for CommitConfig {
}

impl ::protobuf::reflect::ProtobufValue for CommitConfig {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Enum(self.descriptor())
    }
}

#[derive(Clone,PartialEq,Eq,Debug,Hash)]
pub enum StatusCode {
    SUCCESS = 0,
    UNKNOWN_ERROR = 1,
    TYPE_MISMATCH = 2,
    COMMAND_TRANSLATION_ERROR = 3,
    KEY_NOT_FOUND = 4,
    EXTERNAL_PROCESS_ERROR = 5,
    INVALID_KEY = 6,
    FAILED_ROLLBACK = 7,
    LOCKED_QUEUE = 8,
    HOOK_NOT_FOUND = 9,
    SUCCESS_KEY_CREATED = 10,
    MESSAGE_SIZE_ERROR = 11,
}

impl ::protobuf::ProtobufEnum for StatusCode {
    fn value(&self) -> i32 {
        *self as i32
    }

    fn from_i32(value: i32) -> ::std::option::Option<StatusCode> {
        match value {
            0 => ::std::option::Option::Some(StatusCode::SUCCESS),
            1 => ::std::option::Option::Some(StatusCode::UNKNOWN_ERROR),
            2 => ::std::option::Option::Some(StatusCode::TYPE_MISMATCH),
            3 => ::std::option::Option::Some(StatusCode::COMMAND_TRANSLATION_ERROR),
            4 => ::std::option::Option::Some(StatusCode::KEY_NOT_FOUND),
            5 => ::std::option::Option::Some(StatusCode::EXTERNAL_PROCESS_ERROR),
            6 => ::std::option::Option::Some(StatusCode::INVALID_KEY),
            7 => ::std::option::Option::Some(StatusCode::FAILED_ROLLBACK),
            8 => ::std::option::Option::Some(StatusCode::LOCKED_QUEUE),
            9 => ::std::option::Option::Some(StatusCode::HOOK_NOT_FOUND),
            10 => ::std::option::Option::Some(StatusCode::SUCCESS_KEY_CREATED),
            11 => ::std::option::Option::Some(StatusCode::MESSAGE_SIZE_ERROR),
            _ => ::std::option::Option::None
        }
    }

    fn values() -> &'static [Self] {
        static values: &'static [StatusCode] = &[
            StatusCode::SUCCESS,
            StatusCode::UNKNOWN_ERROR,
            StatusCode::TYPE_MISMATCH,
            StatusCode::COMMAND_TRANSLATION_ERROR,
            StatusCode::KEY_NOT_FOUND,
            StatusCode::EXTERNAL_PROCESS_ERROR,
            StatusCode::INVALID_KEY,
            StatusCode::FAILED_ROLLBACK,
            StatusCode::LOCKED_QUEUE,
            StatusCode::HOOK_NOT_FOUND,
            StatusCode::SUCCESS_KEY_CREATED,
            StatusCode::MESSAGE_SIZE_ERROR,
        ];
        values
    }

    fn enum_descriptor_static(_: ::std::option::Option<StatusCode>) -> &'static ::protobuf::reflect::EnumDescriptor {
        static mut descriptor: ::protobuf::lazy::Lazy<::protobuf::reflect::EnumDescriptor> = ::protobuf::lazy::Lazy {
            lock: ::protobuf::lazy::ONCE_INIT,
            ptr: 0 as *const ::protobuf::reflect::EnumDescriptor,
        };
        unsafe {
            descriptor.get(|| {
                ::protobuf::reflect::EnumDescriptor::new("StatusCode", file_descriptor_proto())
            })
        }
    }
}

impl ::std::marker::Copy for StatusCode {
}

impl ::protobuf::reflect::ProtobufValue for StatusCode {
    fn as_ref(&self) -> ::protobuf::reflect::ProtobufValueRef {
        ::protobuf::reflect::ProtobufValueRef::Enum(self.descriptor())
    }
}

static file_descriptor_proto_data: &'static [u8] = b"\
    \n\x11sysadminctl.proto\x12\x0bsysadminctl\"\x1f\n\tInt32List\x12\x12\n\
    \x04list\x18\x01\x20\x03(\x05R\x04list\"\x1e\n\x08BoolList\x12\x12\n\x04\
    list\x18\x01\x20\x03(\x08R\x04list\"\x20\n\nStringList\x12\x12\n\x04list\
    \x18\x01\x20\x03(\tR\x04list\"\x92\x02\n\x0bConfigValue\x12\x1c\n\x08int\
    32val\x18\x01\x20\x01(\x05H\0R\x08int32val\x12\x18\n\x06strval\x18\x02\
    \x20\x01(\tH\0R\x06strval\x12\x1a\n\x07boolval\x18\x03\x20\x01(\x08H\0R\
    \x07boolval\x126\n\tint32list\x18\x04\x20\x01(\x0b2\x16.sysadminctl.Int3\
    2ListH\0R\tint32list\x123\n\x08boollist\x18\x05\x20\x01(\x0b2\x15.sysadm\
    inctl.BoolListH\0R\x08boollist\x129\n\nstringlist\x18\x06\x20\x01(\x0b2\
    \x17.sysadminctl.StringListH\0R\nstringlistB\x07\n\x05value\"G\n\x03Set\
    \x12\x10\n\x03key\x18\x01\x20\x02(\tR\x03key\x12.\n\x05value\x18\x02\x20\
    \x02(\x0b2\x18.sysadminctl.ConfigValueR\x05value\"\x17\n\x03Get\x12\x10\
    \n\x03key\x18\x01\x20\x02(\tR\x03key\";\n\x06Commit\x121\n\x06config\x18\
    \x01\x20\x01(\x0e2\x19.sysadminctl.CommitConfigR\x06config\"\x06\n\x04Dr\
    op\"\x0b\n\tFireHooks\"\x1a\n\x08Rollback\x12\x0e\n\x02id\x18\x01\x20\
    \x02(\rR\x02id\"\x1c\n\x08EraseKey\x12\x10\n\x03key\x18\x01\x20\x02(\tR\
    \x03key\"\x07\n\x05Reset\"\x0b\n\tDumpHooks\"!\n\x0bTriggerHook\x12\x12\
    \n\x04hook\x18\x01\x20\x02(\tR\x04hook\"\x19\n\x05Blame\x12\x10\n\x03key\
    \x18\x01\x20\x02(\tR\x03key\"\n\n\x08InFlight\"\xe7\x04\n\x07Command\x12\
    $\n\x03set\x18\x01\x20\x01(\x0b2\x10.sysadminctl.SetH\0R\x03set\x12$\n\
    \x03get\x18\x02\x20\x01(\x0b2\x10.sysadminctl.GetH\0R\x03get\x12-\n\x06c\
    ommit\x18\x03\x20\x01(\x0b2\x13.sysadminctl.CommitH\0R\x06commit\x12'\n\
    \x04drop\x18\x04\x20\x01(\x0b2\x11.sysadminctl.DropH\0R\x04drop\x126\n\t\
    firehooks\x18\x07\x20\x01(\x0b2\x16.sysadminctl.FireHooksH\0R\tfirehooks\
    \x12-\n\x05erase\x18\x08\x20\x01(\x0b2\x15.sysadminctl.EraseKeyH\0R\x05e\
    rase\x123\n\x08rollback\x18\x0b\x20\x01(\x0b2\x15.sysadminctl.RollbackH\
    \0R\x08rollback\x12*\n\x05reset\x18\x0c\x20\x01(\x0b2\x12.sysadminctl.Re\
    setH\0R\x05reset\x12,\n\x04dump\x18\r\x20\x01(\x0b2\x16.sysadminctl.Dump\
    HooksH\0R\x04dump\x124\n\x07trigger\x18\x0e\x20\x01(\x0b2\x18.sysadminct\
    l.TriggerHookH\0R\x07trigger\x12*\n\x05blame\x18\x0f\x20\x01(\x0b2\x12.s\
    ysadminctl.BlameH\0R\x05blame\x123\n\x08inflight\x18\x10\x20\x01(\x0b2\
    \x15.sysadminctl.InFlightH\0R\x08inflight\x12\x0e\n\x02id\x18\x06\x20\
    \x02(\rR\x02id\x12\x10\n\x03xid\x18\n\x20\x01(\rR\x03xidB\t\n\x07payload\
    \"O\n\x0bMappedField\x12\x10\n\x03key\x18\x01\x20\x02(\tR\x03key\x12.\n\
    \x05value\x18\x02\x20\x01(\x0b2\x18.sysadminctl.ConfigValueR\x05value\"9\
    \n\x0bGetResponse\x12*\n\x03kvs\x18\x01\x20\x03(\x0b2\x18.sysadminctl.Ma\
    ppedFieldR\x03kvs\"-\n\x0eCommitResponse\x12\x1b\n\tcommit_id\x18\x01\
    \x20\x02(\rR\x08commitId\",\n\rResetResponse\x12\x1b\n\tcommit_id\x18\
    \x01\x20\x01(\rR\x08commitId\"X\n\x0cDumpResponse\x12$\n\rtemplatehooks\
    \x18\x01\x20\x03(\tR\rtemplatehooks\x12\"\n\x0cservicehooks\x18\x02\x20\
    \x03(\tR\x0cservicehooks\"v\n\nBlameEntry\x12\x1b\n\tcommit_id\x18\x01\
    \x20\x02(\rR\x08commitId\x12\x1f\n\x0bcommit_time\x18\x02\x20\x02(\tR\nc\
    ommitTime\x12*\n\x03val\x18\x03\x20\x02(\x0b2\x18.sysadminctl.ConfigValu\
    eR\x03val\"T\n\rBlameResponse\x12\x10\n\x03key\x18\x01\x20\x02(\tR\x03ke\
    y\x121\n\x07entries\x18\x02\x20\x03(\x0b2\x17.sysadminctl.BlameEntryR\
    \x07entries\">\n\x10InFlightResponse\x12*\n\x03kvs\x18\x01\x20\x03(\x0b2\
    \x18.sysadminctl.MappedFieldR\x03kvs\"\x91\x03\n\x08Response\x12,\n\x03g\
    et\x18\x01\x20\x01(\x0b2\x18.sysadminctl.GetResponseH\0R\x03get\x125\n\
    \x06commit\x18\x02\x20\x01(\x0b2\x1b.sysadminctl.CommitResponseH\0R\x06c\
    ommit\x122\n\x05reset\x18\x03\x20\x01(\x0b2\x1a.sysadminctl.ResetRespons\
    eH\0R\x05reset\x12/\n\x04dump\x18\x04\x20\x01(\x0b2\x19.sysadminctl.Dump\
    ResponseH\0R\x04dump\x122\n\x05blame\x18\x05\x20\x01(\x0b2\x1a.sysadminc\
    tl.BlameResponseH\0R\x05blame\x12;\n\x08inflight\x18\x08\x20\x01(\x0b2\
    \x1d.sysadminctl.InFlightResponseH\0R\x08inflight\x12\x0e\n\x02id\x18\
    \x06\x20\x02(\rR\x02id\x12/\n\x06status\x18\x07\x20\x02(\x0e2\x17.sysadm\
    inctl.StatusCodeR\x06statusB\t\n\x07payload*<\n\x0cCommitConfig\x12\x0b\
    \n\x07DEFAULT\x10\0\x12\x11\n\rTEMPLATE_ONLY\x10\x01\x12\x0c\n\x08NO_HOO\
    KS\x10\x03*\x8a\x02\n\nStatusCode\x12\x0b\n\x07SUCCESS\x10\0\x12\x11\n\r\
    UNKNOWN_ERROR\x10\x01\x12\x11\n\rTYPE_MISMATCH\x10\x02\x12\x1d\n\x19COMM\
    AND_TRANSLATION_ERROR\x10\x03\x12\x11\n\rKEY_NOT_FOUND\x10\x04\x12\x1a\n\
    \x16EXTERNAL_PROCESS_ERROR\x10\x05\x12\x0f\n\x0bINVALID_KEY\x10\x06\x12\
    \x13\n\x0fFAILED_ROLLBACK\x10\x07\x12\x10\n\x0cLOCKED_QUEUE\x10\x08\x12\
    \x12\n\x0eHOOK_NOT_FOUND\x10\t\x12\x17\n\x13SUCCESS_KEY_CREATED\x10\n\
    \x12\x16\n\x12MESSAGE_SIZE_ERROR\x10\x0b\
";

static mut file_descriptor_proto_lazy: ::protobuf::lazy::Lazy<::protobuf::descriptor::FileDescriptorProto> = ::protobuf::lazy::Lazy {
    lock: ::protobuf::lazy::ONCE_INIT,
    ptr: 0 as *const ::protobuf::descriptor::FileDescriptorProto,
};

fn parse_descriptor_proto() -> ::protobuf::descriptor::FileDescriptorProto {
    ::protobuf::parse_from_bytes(file_descriptor_proto_data).unwrap()
}

pub fn file_descriptor_proto() -> &'static ::protobuf::descriptor::FileDescriptorProto {
    unsafe {
        file_descriptor_proto_lazy.get(|| {
            parse_descriptor_proto()
        })
    }
}
