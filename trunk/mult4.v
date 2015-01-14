module mult4(
	a0,a1,a2,a3,
	b0,b1,b2,b3,
	z0,z1,z2,z3,z4,z5,z6,z7);
input a0,a1,a2,a3,b0,b1,b2,b3;
output z0,z1,z2,z3,z4,z5,z6,z7;
wire x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17;

assign z0 = a0&b0;
//HA
assign z1 = (a1&b0)^(a0&b1);
assign x1 = (a1&b0)&(a0&b1);
//FA
assign x2 = (a1&b2)^(a0&b2)^x1;
assign x3 = ((a1&b2)&(a0&b2)) | ((a1&b2)&x1) | ((a0&b2)&x1);
//FA
assign x4 = (a1&b2)^(a0&b3)^x3;
assign x5 = ((a1&b2)&(a0&b3)) | ((a1&b2)&x3) | ((a0&b3)&x3);
//HA
assign x6 = (a1&b3)^x5;
assign x7 = (a1&b3)&x5;
//HA
assign z2 = x2^(a2&b0);
assign x15 = x2&(a2&b0);
//FA
assign x14 = x4^(a2&b1)^x15;
assign x16 = (x4&(a2&b1)) | (x4&x15) | ((a2&b1)&x15);
//FA
assign x13 = x6^(a2&b1)^x15;
assign x17 = (x6&(a2&b1)) | (x6&x15) | ((a2&b1)&x15);
//FA
assign x9 = x7^(a2&b3)^x17;
assign x8 = (x7&(a2&b3)) | (x7&x17) | ((a2&b3)&x17);
//ha
assign z3 = x14^(a3&b0);
assign x12 = x14&(a3&b0);
//fa
assign z4 = x13^(a3&b1)^x12;
assign x11 = (x13&(a3&b1)) | (x13&x12) | ((a3&b1)&x12);
//fa
assign z5 = x9^(a3&b2)^x11;
assign x10 = (x9&(a3&b2)) | (x9&x11) | ((a3&b2)&x11);
//fa
assign z6 = x8^(a3&b3)^x10;
assign z7 = (x8&(a3&b3)) | (x8&x10) | ((a3&b3)&x10);

endmodule


