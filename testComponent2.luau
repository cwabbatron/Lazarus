
component = {
    name = "TestComponent 2",
    value = 0,
    on_update = function(self)
        print(`TestComponent2 running with value {self.value}`)
        self.value += 2
    end,
    on_attach = function(self)
        print("attached")
    end,

    on_deattach = function(self)
        print("GONE")
    end,
}