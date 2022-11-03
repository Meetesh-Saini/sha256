import basics
from constants import constants, initials

def sha256(text):
    if isinstance(text, bytes):
        message = "".join(map(lambda x: bin(x)[2:].zfill(8), text))
    else:
        message = "".join(map(lambda x: bin(ord(x))[2:].zfill(8), text))

    # padding message
    message = message+'1'
    len_message = len(message)
    padding_space = len_message % 512
    padding_div = len_message//512
    padded_message = message
    if padding_space > 448:
        padded_message = message + '0'*((padding_div+1)*512+448 - len_message)
    elif padding_space < 448:
        padded_message = message + '0'*(padding_div*512+448 - len_message)

    # padding with length
    padded_message += bin(len_message-1)[2:].zfill(64)

    # message blocks
    block_number = 0
    total_blocks = len(padded_message) // 512
    # initial hash value
    hash_initials = initials.copy()

    while block_number < total_blocks:
        start_index = block_number*512
        end_index = start_index + 512

        # message block to hash
        block = padded_message[start_index:end_index]

        # buffers:
        # a b c d e f g h
        # 0 1 2 3 4 5 6 7
        buffer_values = hash_initials.copy()
        
        # message schedule
        schedule_list = []
        for i in range(64):
            if i < 16:
                message_schedule = int(block[i*32:i*32+32],2)
            else:
                # message_schedule = basics.add(basics.add(basics.add(basics.sigama1(schedule_list[i-2]), schedule_list[i-7]), basics.sigma0(schedule_list[i-15])), schedule_list[i-16])
                message_schedule = basics.sigama1(schedule_list[i-2]) + schedule_list[i-7] + basics.sigma0(schedule_list[i-15]) + schedule_list[i-16]
                message_schedule = message_schedule % 2**32
            schedule_list.append(message_schedule)

            t1 = basics.SIGMA1(buffer_values[4]) + basics.choice(buffer_values[4], buffer_values[5], buffer_values[6]) + buffer_values[7] + constants[i] + message_schedule
            t2 = basics.SIGMA0(buffer_values[0]) + basics.majority(buffer_values[0], buffer_values[1], buffer_values[2])
            t1 = t1 % 2**32
            t2 = t2 % 2**32

            # rotate right 1 time and chnaging value of 'a'
            buffer_values.insert(0, basics.add(t1,t2))
            buffer_values.pop()

            # changing value of 'e'
            buffer_values[4] = basics.add(buffer_values[4], t1)
        
        hash_initials = [basics.add(hash_initials[i], buffer_values[i]) for i in range(8)]
        block_number += 1     

    return "".join(map(lambda x: hex(x)[2:].zfill(8), hash_initials))