def sum(number_one, number_two):
    number_one_init = conver_interger(number_one)
    number_two_init = conver_interger(number_two)

    return number_one_init + number_two_init


def conver_interger(number_string):
    convered_interger = int(number_string)
    return convered_interger



answer=sum("1", "2")

print answer