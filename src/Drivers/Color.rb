@class = "20"
@desc = "Color sensing driver"

@members = lambda do
  out %{
    unsigned char a;
  }
end

@init = lambda do
  out %{
    self->a = 0xE;
    DebugPrint(self->a);
  }
end

@respond = lambda do
  out %{
  }
end

@poll = lambda do
  out %{
   unsigned char data[1];
   data[0] = 0x2;
   I2CReadRequest(self->id, 0x30, data, 1);
   start_UART_send(1, data);
  }
end
