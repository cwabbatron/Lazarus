
component = {
    name = "MyLuaComponent",
    value = 0,
    on_update = function(self)
        print(`i am {self.name} with value {self.value}\n`)
        self.value += 1
        self.test("running test function with dot")
        

        self:test("running test function with colon")
    end,
    on_attach = function(self)
        print("attached")
    end,

    on_deattach = function(self)
        print("GONE")
    end,
}