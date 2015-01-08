// 32-bit Ripple-Carry Adder
module adder32 (cout,
	s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,
	a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,
	b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15);
   
   input a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15;
   input b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15;
   output s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15;
   output cout;         // carry out bit
   wire c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15;

assign s0 = a0 ^ b0 ^ 1'b0;
assign c1 = (a0&b0);
assign c2 = (a1&b1)|(a1&c1)|(b1&c1);
assign s1 = a1 ^ b1 ^ c1;
assign c3 = (a2&b2)|(a2&c2)|(b2&c2);

assign s2 = a2 ^ b2 ^ c2;
assign c4 = (a3&b3)|(a3&c3)|(b3&c3);

assign s3 = a3 ^ b3 ^ c3;
assign c5 = (a4&b4)|(a4&c4)|(b4&c4);

assign s4 = a4 ^ b4 ^ c4;
assign c6 = (a5&b5)|(a5&c5)|(b5&c5);

assign s5 = a5 ^ b5 ^ c5;
assign c7 = (a6&b6)|(a6&c6)|(b6&c6);

assign s6 = a6 ^ b6 ^ c6;
assign c8 = (a7&b7)|(a7&c7)|(b7&c7);

assign s7 = a7 ^ b7 ^ c7;
assign c9 = (a8&b8)|(a8&c8)|(b8&c8);

assign s8 = a8 ^ b8 ^ c8;
assign c10 = (a9&b9)|(a9&c9)|(b9&c9);

assign s9 = a9 ^ b9 ^ c9;
assign c11 = (a10&b10)|(a10&c10)|(b10&c10);

assign s10 = a10 ^ b10 ^ c10;
assign c12 = (a11&b11)|(a11&c11)|(b11&c11);

assign s11 = a11 ^ b11 ^ c11;
assign c13 = (a12&b12)|(a12&c12)|(b12&c12);

assign s12 = a12 ^ b12 ^ c12;
assign c14 = (a13&b13)|(a13&c13)|(b13&c13);

assign s13 = a13 ^ b13 ^ c13;
assign c15 = (a14&b14)|(a14&c14)|(b14&c14);

assign s14 = a14 ^ b14 ^ c14;
assign cout = (a15&b15)|(a15&c15)|(b15&c15);

assign s15 = a15 ^ b15 ^ c15;

endmodule
