/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

export const BASENAME = import.meta.env.VITE_BASE_NAME || '/';

export const DEFAULTCONFIG = {
  resolution: {
    width: 640,
    height: 360,
  },
  resolutionsText: '640 * 480',
  frameRate: 15,
  bitrate: {
    min: 800,
    max: 800,
  },
};

export const RESOLUTIOIN_LIST = [
  {
    text: '640 * 360',
    val: {
      width: 640,
      height: 360,
    },
    bitrateRange: {
      min: 200,
      max: 1000,
    },
  },
  {
    text: '640 * 480',
    val: {
      width: 640,
      height: 480,
    },
    bitrateRange: {
      min: 250,
      max: 1000,
    },
  },
  {
    text: '960 * 540',
    val: {
      width: 960,
      height: 540,
    },
    bitrateRange: {
      min: 400,
      max: 1600,
    },
  },
  {
    text: '1280 * 720',
    val: {
      width: 1280,
      height: 720,
    },
    bitrateRange: {
      min: 500,
      max: 2000,
    },
  },
  {
    text: '1920 * 1080',
    val: {
      width: 1920,
      height: 1080,
    },
    bitrateRange: {
      min: 800,
      max: 3000,
    },
  },
];

export const Frame = [15, 20, 24, 30, 60];

export const DEMO_VERSION = '1.0.0';
