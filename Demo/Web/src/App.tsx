/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { BrowserRouter, Routes, Route } from 'react-router-dom';
import Push from '@/pages/Push';
import View from '@/pages/View';
import { BASENAME } from './config';

function App() {
  return (
    <BrowserRouter basename={BASENAME}>
      <Routes>
        <Route path="/">
          <Route index element={<Push />} />
          <Route path="view" element={<View />} />
        </Route>
      </Routes>
    </BrowserRouter>
  );
}

export default App;
