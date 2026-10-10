type Entity = {
    get_components: (self:Entity) -> {Component},

    --TODO: add this add_component: (self:Entity,component: Component) -> (),
    --or this        add_component: (self:Entity,component_name: string) -> (),

    get_component: (self:Entity,component_name: string) -> Component,

    has_componet: (self:Entity,component_name: string) -> boolean,
}
function read_file(file_path: string): string return "" end

function write_file(file_path: string, content: string): () end
type Component = {
    update: (self: Component,attachment: Entity) -> (),
    
    get_property: (self:Component,name: string) -> any,
    get_property_typed: <T>(self:Component,name: string) -> T,

    set_property: (self:Component, name:string, value: any) -> (),
}