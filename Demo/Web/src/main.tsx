/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import ReactDOM from 'react-dom/client';
import { Provider } from 'react-redux';
import App from './App';
import store from './store';
import './index.less';
import './react-i18next-config';

ReactDOM.createRoot(document.getElementById('root')!).render(
  <Provider store={store}>
    <App />
  </Provider>
);
