#ifndef ESPI_GPIO_H
#define ESPI_GPIO_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define assert_syscall(sc) do { if ((sc) < 0) { printf ("syscall failed("#sc")\n"); exit(EXIT_FAILURE);} } while(0)

#define array_size(a) (sizeof(a) / sizeof(*a))
#define assert_msg(cnd, msg) do { if ((!cnd)) { puts(msg); exit(EXIT_FAILURE);}}while (0)
#define _inline __attribute__((always_inline))

#define BCM2708_PERI_BASE 0x3F000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000)

#define GPIO_INPUT(m, pin) *(m.mmio_buff + ((pin)/10)) &= ~(7<<(((pin)%10)*3))
#define GPIO_OUTPUT(m, pin) *(m.mmio_buff + ((pin)/10)) |= (1<<(((pin)%10)*3))
#define GPIO_SET(m, pin) *(m.mmio_buff + 7) = 1<<pin
#define GPIO_CLR(m, pin) *(m.mmio_buff + 10) = 1<<pin
#define GPIO_READ(m, pin) (*(m.mmio_buff + 13) & (1<<pin))

#define RPI_3_PINS  {			    \
    2, 	3,  4,  17, 27, 22, 10,		\
    9, 	11, 0,  5,  6,  13, 19,		\
    26, 14, 15, 18, 23, 24, 25,		\
    8,  7,  1,  12, 16, 20, 21,		\
}

enum modes {
    GPIO_HIGH = 0,
    GPIO_LOW,
    GPIO_IN,
    GPIO_OUT,
};

typedef struct {
    uint32_t *mmio_buff;
    size_t gpio_fd;
} GPIO_Mem;

volatile GPIO_Mem map_gpio();
void gpio_set_output(const size_t gpio_pin);
int is_valid_pin(size_t pin);
size_t get_hardware_pin(size_t logical);

#ifdef ESPI_GPIO_IMPL

static const size_t gpio_pins[] = RPI_3_PINS;

volatile GPIO_Mem map_gpio()
{
    volatile GPIO_Mem mem;

    assert_syscall(mem.gpio_fd = open("/dev/gpiomem", O_RDWR | O_SYNC));
    assert_syscall(mem.mmio_buff = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem.gpio_fd, GPIO_BASE));

    return mem;
}

int is_valid_pin(size_t pin)
{
    return (pin <= array_size(gpio_pins));
}

size_t get_hardware_pin(size_t logical)
{
    return gpio_pins[logical];
}

#endif /* ESPI_GPIO_IMPL */
#endif /* ESPI_GPIO_H */
