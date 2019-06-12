#!/usr/bin/env python3

import argparse
import tensorflow as tf
import numpy as np

argparser = argparse.ArgumentParser()
argparser.add_argument("--width", default=1, type=int)
argparser.add_argument("--height", default=1, type=int)
argparser.add_argument("--channels", default=1, type=int)
argparser.add_argument("--kernel_width", default=1, type=int)
argparser.add_argument("--kernel_height", default=1, type=int)
argparser.add_argument("--features", default=1, type=int)
args = argparser.parse_args()

print("width", args.width)
print("height", args.height)
print("channels", args.channels)
print("kernel_width", args.kernel_width)
print("kernel_height", args.kernel_height)
print("features", args.features)

inp_var = tf.get_variable(shape=(1, args.height, args.width, args.channels), name="inp")
dweights_var = tf.get_variable(shape=(args.kernel_height, args.kernel_width, args.channels, 1), name="dweights")
pweights_var = tf.get_variable(shape=[1, 1, args.channels, args.features], name="pweights")

output = tf.nn.separable_conv2d(
    inp_var,
    dweights_var,
    pweights_var,
    strides=[1,1,1,1],
    padding='SAME',
)

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())

    inp = np.random.rand(1, args.height, args.width, args.channels)
    dweights = np.random.rand(args.kernel_height, args.kernel_width, args.channels, 1)
    pweights = np.random.rand(1, 1, args.channels, args.features)

    print("input", end="")
    for c in range(args.channels):
        for y in range(args.height):
            for x in range(args.width):
                print("", inp[0][y][x][c], end="")
    print("")

    print("dweights", end="")
    for c in range(args.channels):
        for y in range(args.kernel_height):
            for x in range(args.kernel_width):
                print("", dweights[y][x][c][0], end="")
    print("")

    print("pweights", end="")
    for f in range(args.features):
        for c in range(args.channels):
            print("", pweights[0][0][c][f], end="")
    print("")

    out = sess.run(output, feed_dict={inp_var: inp, dweights_var: dweights, pweights_var: pweights})
    print("output", end="")
    for f in range(args.features):
        for y in range(args.height):
            for x in range(args.width):
                print("", inp[0][y][x][f], end="")
    print("")
