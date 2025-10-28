#include "philo.h"

static int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

long	ft_atol(const char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res);
}

static int	is_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-' || str[0] == '\0')
		return (0);
	if (str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_args(int argc, char **argv)
{
	int		i;
	long	val;

	i = 1;
	if (argc != 5 && argc != 6)
		return (printf("Error: wrong number of arguments\n"), 0);
	while (i < argc)
	{
		if (!is_number(argv[i]))
			return (printf("Error: invalid input: %s\n", argv[i]), 0);
		val = ft_atol(argv[i]);
		if ((i < 5 && val <= 0) || val > INT_MAX)
			return (printf("Error: invalid range: %s\n", argv[i]), 0);
		if (i == 1 && val > 200)
			return (printf("Error: too many philosophers (max 200)\n"), 0);
		i++;
	}
	return (1);
}
