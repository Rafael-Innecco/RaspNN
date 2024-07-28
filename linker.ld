SECTIONS {
  . = 0x400000;
  .text : { 
     *(.text) 
  }

  . = ALIGN(4);
  .data : { *(.data) }

  . = ALIGN(4);
  bss_begin = .;
  .bss : { *(.bss*) }
  bss_end = .;

  . = ALIGN(8);
  heap_addr = .;
  . = . + 32K;
  stack_user = .;
  . = . + 32K;
  stack_svr = .;
}
