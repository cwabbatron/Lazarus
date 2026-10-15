type Entity = {
    get_components: (self:Entity) -> {Component},

    add_component: (self:Entity, component_name: string) -> (),

    get_component: (self:Entity,component_name: string) -> Component,

    has_componet: (self:Entity,component_name: string) -> boolean,
}
function read_file(file_path: string): string return "" end

function write_file(file_path: string, content: string): () end
type Component = {
    update: (self: Component,attachment: Entity) -> (),
    
    on_attach: (self: Component, attachment: Entity) -> (),
    on_deattach: (self: Component, attachment: Entity) -> (),
    name: string,
    
    get_property: (self:Component,name: string) -> any,
    get_property_typed: <T>(self:Component,name: string) -> T,

    set_property: (self:Component, name:string, value: any) -> (),
}