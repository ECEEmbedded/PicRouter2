@class = "20"
@desc = "Color sensing driver"

@members = lambda do
  out %{
    unsigned char a;
  }
end

@init = lambda do
  out %{
    
  }
end

@respond = lambda do
  out %{
  }
end

@poll = lambda do
  out %{
    unsigned char data[3];
    I2CReadRequest(self->id, 0x10, data, 3);
    start_UART_send(3, &data[0]);
   }
end
